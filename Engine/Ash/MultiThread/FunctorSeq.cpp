#include "FunctorSeq.h"

#include "Ash/MultiThread/ThreadPool.h"

void Ash::FunctorSeq::entry(const Functor& functor)
{
	runFunctor(functor, nullptr);
}

void Ash::FunctorSeq::setDebugName(std::string debugName)
{
#ifdef TINY_DEBUG
	_debugName = std::move(debugName);
#endif
}

//注意submit可能delete this
void Ash::FunctorSeq::submit()
{
	//单线程进来
	submitNextFunctor(true);//注意submitNextFunctor中可能会delete this
}

void Ash::FunctorSeq::onFinishFunctor(FunctorSeq* subSeq)
{
	//多线程进来，但是每个进来的线程的subSeq一定是当前线程自己new的
	
	if (subSeq && !subSeq->empty())
	{
		// 有subSeq，不减少_runningFunctor，我们认为这个functor还没running完，当这个subseq完成时，回调到onSubSeqFinish中去submitNextFunctor
		subSeq->submit();
	}
	else
	{
		submitNextFunctor(false);
	}
}

void Ash::FunctorSeq::onSubSeqFinish()
{
	submitNextFunctor(false);
}

//注意submitNextFunctor中可能会delete this
//isInitialSubmit: 如果是初始submit，则一定是单线程进来，并且无视_runningFunctor强制提交下一个functor
void Ash::FunctorSeq::submitNextFunctor(bool isInitialSubmit) 
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
			doSubmitNextFunctor();
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
			}
			delete this; //删除自己，注意接下来不能再访问this
		}
	}
}

void Ash::FunctorSeq::doSubmitNextFunctor()
{
    //一定是单线程进来
	BeginFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);
	TinyAssert(_runningFunctor == 0);
	TinyAssert(!empty());

	//找到要提交的范围
	int start = _currentFunctor;
	int end = start;
	for (;end < static_cast<int>(_functors.size()) && _functors[end]; ++end){}
	TinyAssert(end > start);
	_currentFunctor = end;
	if(_currentFunctor < static_cast<int>(_functors.size()))
	{
		++_currentFunctor; //跳过nullptr分隔符
	}

	//通过修改_runningFunctor，使得仅当最后一个functor执行完毕时，this才会被删除
	_runningFunctor = end - start;
	
	//接下来一旦提交了task，这个函数就可能多线程访问了，重置这个assert FLAG
	EndpeFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);

	//注意接下来一旦提交后，this就可能因为完成了所有functor而被析构，一旦提交后就不能再访问this了
	
	if (end - start == 1)
	{
		//如果只有一个functor，直接在本线程执行
		runFunctor(_functors[start], this);
	}
	else
	{
		for (; start < end; ++start)
		{
            //注意ThreadPool不负责_functors的生命周期
			ThreadPool::instance()->dispatchFunctor(&FunctorSeq::runFunctor, &_functors[start], this);
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
		_functors.emplace_back(std::move(functor));
	}
}

void Ash::FunctorSeq::runFunctor(const Functor& functor, FunctorSeq* parent)
{
	FunctorSeq* seq = new FunctorSeq();
	seq->_parent = parent;
	functor(*seq);
	if(parent)
	{
		parent->onFinishFunctor(seq);
	}
	else
	{
		seq->submit();
	}
}
