#include "FunctorSeq.h"

#include "Ash/MultiThread/ThreadPool.h"

using FunctorSeqPtr = Ash::RefCountPtr<Ash::FunctorSeq>;

void Ash::FunctorSeq::entry(const Functor& functor)
{
	FunctorSeqPtr seq = Ash::MakeRefCountPtr<FunctorSeq>();
	functor(*seq);
	seq->submit(true);
}

Ash::FunctorSeq& Ash::FunctorSeq::then(std::vector<Functor> functors)
{
	if (functors.empty())
		return *this;
	
	_functors.reserve(_functors.size() + functors.size());
	pushSeparatorFunctor();
	const auto nowSize = _functors.size();
	for(Functor& fun : functors)
	{
		if (fun)
			pushThenFunctor(std::move(fun));
	}
	if(nowSize == _functors.size())
	{
		_functors.pop_back();//pop separator
	}
	return *this;
}

Ash::Future Ash::FunctorSeq::future(Functor functor)
{
	FunctorSeqPtr seq = MakeRefCountPtr<FunctorSeq>();
	pushFutureFunctor(std::move(functor), seq.get());
	return Future{ std::move(seq) };
}

Ash::FunctorSeq& Ash::FunctorSeq::loop(std::function<bool()> pred, Functor functor)
{
	pushLoopFunctor(std::move(functor), std::move(pred));
	return *this;
}

void Ash::FunctorSeq::setDebugName(std::string debugName)
{
#ifdef TINY_DEBUG
	_debugName = std::move(debugName);
#endif
}

void Ash::FunctorSeq::submit(bool forceAsync/* = false*/)
{
	//单线程进来
	addRef();//这里+1，在submitNextFunctor时，如果空了就-1
	submitNextFunctor(true, forceAsync);//注意submitNextFunctor中可能会releaseRef
}

void Ash::FunctorSeq::onFinishFunctor(FunctorSeq* newRecordedSeq, const FunctorSaving& theFinishedFunctor)
{
	//多线程进来，但是每个进来的线程的newRecordedSeq一定是当前线程自己new的
	TinyAssert(newRecordedSeq);
	switch (theFinishedFunctor._functorType)
	{
	case FunctorSaving::FunctorType::ThenFunctor:
	{
		// 有subSeq，不减少_runningFunctor，我们认为这个functor还没running完，当这个subseq完成时，回调到onSubSeqFinish中去submitNextFunctor
		newRecordedSeq->submit();
		break;
	}
	case FunctorSaving::FunctorType::FutureFunctor:
	{
		//期货，那么提交它，和自己没啥关系，自己直接submitNextFunctor
		newRecordedSeq->submit(true);
		TinyAssert(newRecordedSeq->_parent == nullptr);
		submitNextFunctor(false, false);
		break;
	}
	case FunctorSaving::FunctorType::Future:
	{
		std::unique_lock<SpinMutex> lock(newRecordedSeq->_futureMutex); //锁范围：newRecordedSeq->_parent和_futureHasFinished
		TinyAssert(newRecordedSeq->_parent == nullptr);
		newRecordedSeq->_parent = this; //修复期货的parent
		if (newRecordedSeq->_futureHasFinished)
			onSubSeqFinish(); //本该由newRecordedSeq调用，但是当时它的parent为空，没有调用
		break;
	}
	case FunctorSaving::FunctorType::LoopFunctor:
	{
			if(newRecordedSeq)

		break;
	}
	default:
		TinyAssert(false);
	}
	
}

void Ash::FunctorSeq::onSubSeqFinish()
{
	submitNextFunctor(false, false);
}

//注意submitNextFunctor中可能会delete this
//isInitialSubmit: 如果是初始submit，则一定是单线程进来，并且无视_runningFunctor强制提交下一个functor
void Ash::FunctorSeq::submitNextFunctor(bool isInitialSubmit, bool forceAsync)
{
	bool shouldSubmitNext = false;
    if(isInitialSubmit)
    {
        //单线程进来，但可能subseq完成后递归进来多次到这
		shouldSubmitNext = true;//初始submit无视_runningFunctor，直接提交
    }
	else 
	{
		//多线程进来
		int running = --_runningFunctor;
		TinyAssert(running >= 0);
		shouldSubmitNext = (running == 0); //当_runningFunctor等于0时，本线程就是最后一个跑this的线程，所以接下来可以认为单线程
	}
	if (shouldSubmitNext)
	{
		//单线程进来，但可能subseq完成后递归进来多次到这
		if (!empty())
		{
			doSubmitNextFunctor(forceAsync);
		}
		else
		{
			{
				//这里开始单线程
				ScopeFlagAssert(_singleThreadVisitChecker_trySubmitNextFunctor);
				//本seq完全执行完毕，向上报告
				if (_parent)
				{
					_parent->onSubSeqFinish();
				}
				else
				{
					//parent为空，说明要么是entry()进来的，要么是期货。entry进来的这里无需向上报告，但是如果是期货，我们需要延迟报告的时间知道Future被执行
					std::unique_lock<SpinMutex> lock(_futureMutex); //锁的目标：newRecordedSeq->_parent和_futureHasFinished这两个变量，为了和onFinishFunctor不冲突
					if (_parent)
					{
						_parent->onSubSeqFinish();
					}
					else
					{
						_futureHasFinished = true;
					}
				}
			}
			releaseRef(); //可能删除自己，注意接下来不能再访问this
		}
	}
}

void Ash::FunctorSeq::doSubmitNextFunctor(bool forceAsync)
{
    //一定是单线程进来
	BeginFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);
	TinyAssert(_runningFunctor == 0);
	TinyAssert(!empty());

	//找到要提交的范围
	int start = _currentFunctor;
	int end = start;
	for (; end < static_cast<int>(_functors.size()) && _functors[end]._functorType != FunctorSaving::FunctorType::Separator; ++end) {}
	TinyAssert(end > start);
	_currentFunctor = end;
	if (_currentFunctor < static_cast<int>(_functors.size()))
	{
		++_currentFunctor; //跳过分隔符
	}

	//通过修改_runningFunctor，使得仅当最后一个functor执行完毕时，this才会被删除
	_runningFunctor = end - start;

	//接下来一旦提交了task，这个函数就可能多线程访问了，重置这个assert FLAG
	EndpeFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);

	//注意接下来一旦提交后，this就可能因为完成了所有functor而被析构，一旦提交后就不能再访问this了

	if (end - start == 1 && !forceAsync)
	{
		//如果只有一个functor，直接在本线程执行
		runFunctor(_functors[start]);
	}
	else
	{
		for (; start < end; ++start)
		{
			//注意ThreadPool不负责_functors的生命周期
			ThreadPool::instance()->dispatchFunctor(&FunctorSeq::runFunctor, this, &_functors[start]);
		}
	}
}

bool Ash::FunctorSeq::empty() const
{
	TinyAssert(_currentFunctor <= static_cast<int>(_functors.size()));
	return _currentFunctor == static_cast<int>(_functors.size());
}

void Ash::FunctorSeq::thenImpl(Functor functor)
{
	if (functor)
	{
		pushThenFunctor(std::move(functor));
	}
}

void Ash::FunctorSeq::thenImpl(Future future)
{
	pushFuture(std::move(future));
}

void Ash::FunctorSeq::runFunctor(const FunctorSaving& functor)
{
	switch (functor._functorType)
	{
	case FunctorSaving::FunctorType::ThenFunctor: 
	{
		const FunctorSaving::ThenFactorStruct& save = std::get<FunctorSaving::ThenFactorStruct>(functor._data);
		FunctorSeqPtr seq = MakeRefCountPtr<FunctorSeq>();
		seq->_parent = this;
		save._functor(*seq);
		onFinishFunctor(seq.get(), functor);
		break;
	}
	case FunctorSaving::FunctorType::FutureFunctor:
	{
		const FunctorSaving::FutureFunctorStruct& save = std::get<FunctorSaving::FutureFunctorStruct>(functor._data);
		save._functor(*save._seq);
		onFinishFunctor(save._seq.get(), functor);
		break;
	}
	case FunctorSaving::FunctorType::LoopFunctor:
	{
		const FunctorSaving::LoopFunctorStruct& save = std::get<FunctorSaving::LoopFunctorStruct>(functor._data);
		if (save._pred && save._pred())
		{
			FunctorSeqPtr seq = MakeRefCountPtr<FunctorSeq>();
			seq->_parent = this;
			save._functor(*seq);
			onFinishFunctor(seq.get(), functor);
		}
		else
		{
			onFinishFunctor(nullptr, functor);
		}
		break;
	}
	case FunctorSaving::FunctorType::Future:
	{
		const FunctorSaving::FutureStruct& save = std::get<FunctorSaving::FutureStruct>(functor._data);
		onFinishFunctor(save._seq.get(), functor);
        break;
	}
	default:
		TinyAssert(false);
	}
	
	
}

void Ash::FunctorSeq::pushSeparatorFunctor()
{
	_functors.emplace_back(FunctorSaving::FunctorType::Separator, FunctorSaving::SeparatorStruct{});
}

void Ash::FunctorSeq::pushFutureFunctor(Functor functor, FunctorSeq* seq)
{
	_functors.emplace_back(FunctorSaving::FunctorType::FutureFunctor, FunctorSaving::FutureFunctorStruct{ std::move(functor), FunctorSeqPtr{ seq } });
}

void Ash::FunctorSeq::pushFuture(Future future)
{
	TinyAssert(future._seq.isValid());
    if(future._seq.isValid())
	    _functors.emplace_back(FunctorSaving::FunctorType::Future, FunctorSaving::FutureStruct{ std::move(future._seq) });
}

void Ash::FunctorSeq::pushThenFunctor(Functor functor)
{
	_functors.emplace_back(FunctorSaving::FunctorType::ThenFunctor, FunctorSaving::ThenFactorStruct{ std::move(functor) });
}

void Ash::FunctorSeq::pushLoopFunctor(Functor functor, std::function<bool()> pred)
{
	_functors.emplace_back(FunctorSaving::FunctorType::LoopFunctor, FunctorSaving::LoopFunctorStruct{ std::move(functor) ,std::move(pred) });
}
