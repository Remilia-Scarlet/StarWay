#include "FunctorSeq.h"

#include "Ash/MultiThread/ThreadPool.h"

void Ash::FunctorSeq::entry(Functor functor)
{
	
	FunctorSeqPtr seq { new FunctorSeq() };
	functor(*seq);
	seq->submit();
}

void Ash::FunctorSeq::submit()
{
	//单线程
	if (!empty())
	{
		doSubmitNextFunctor();
		addRef();
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
	ScopeFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);
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
		++_currentFunctor;
	}

	//提交它们
	_runningFunctor = end - start;
	for (; start < end; ++start)
	{
		class Task : public Ash::ThreadPoolTask
		{
		public:
			Task(Functor functor, FunctorSeq* seq) :_functor(std::move(functor)), _seq(seq) {}
			void onRun() override
			{
				Ash::FunctorSeqPtr subSeq{ new Ash::FunctorSeq() };
				subSeq->_parent = _seq;
				_functor(*subSeq);
				_seq->onFinishSeq(subSeq.get());
			}

			Functor _functor;
			FunctorSeq* _seq = nullptr;
		};
		RefCountPtr<Task> task = Ash::MakeRefCountPtr<Task>(std::move(_functors[start]), this);
		ThreadPool::instance()->dispatchTask(task);
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