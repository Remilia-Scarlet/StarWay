#include "FunctorSeq.h"

#include "Ash/MultiThread/ThreadPool.h"

void Ash::FunctorSeq::entry(const Functor& functor)
{
	entry(functor, nullptr);
}

void Ash::FunctorSeq::setDebugName(std::string debugName)
{
#ifdef TINY_DEBUG
	_debugName = std::move(debugName);
#endif
}

void Ash::FunctorSeq::submit()
{
	//单线程
	if (!empty())
	{
		addRef();
		doSubmitNextFunctor();
	}
}

void Ash::FunctorSeq::onFinishSeq(FunctorSeq* subSeq)
{
	//可能多线程同时访问
	
	if (subSeq && !subSeq->empty())
	{
		// 有subSeq，不减少_runningFunctor，我们认为这个functor还没running完，直到这个subseq完成
		subSeq->submit();
	}
	else
	{
		trySubmitNextFunctor();
	}
}

void Ash::FunctorSeq::onSubSeqFinish()
{
	trySubmitNextFunctor();
}

void Ash::FunctorSeq::trySubmitNextFunctor()
{
	//可能多线程进来
	int running = --_runningFunctor;
	TinyAssert(running >= 0);
	if (running == 0)
	{
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
			releaseRef();//可能在此析构，之后不能再访问this
		}
	}
}

void Ash::FunctorSeq::doSubmitNextFunctor()
{
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

	//提交它们
	_runningFunctor = end - start;
	
	//接下来一旦提交了task，这个函数就可能多线程访问了，重置这个assert FLAG
	EndpeFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);

	//注意接下来一旦提交后，this就可能因为完成了所有functor而被析构，一旦提交后就不能再访问this了
	
	if (end - start == 1)
	{
		//如果只有一个functor，直接在本线程执行
		entry(_functors[start], this);
	}
	else
	{
		for (; start < end; ++start)
		{
			class Task : public Ash::ThreadPoolTask
			{
			public:
				Task(Functor functor, FunctorSeq* seq) :_functor(std::move(functor)), _seq(seq) {}
				void onRun() override
				{
					entry(_functor, _seq);
				}

				Functor _functor;
				FunctorSeq* _seq = nullptr;
			};
			RefCountPtr<Task> task = Ash::MakeRefCountPtr<Task>(std::move(_functors[start]), this);
			ThreadPool::instance()->dispatchTask(task);
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

void Ash::FunctorSeq::entry(const Functor& functor, FunctorSeq* parent)
{
	FunctorSeqPtr seq{ new FunctorSeq() };
	seq->_parent = parent;
	functor(*seq);
	if(parent)
	{
		parent->onFinishSeq(seq.get());
	}
	else
	{
		seq->submit();
	}
}
