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
	//���߳�
	if (!empty())
	{
		doSubmitNextFunctor();
		addRef();
	}
}

void Ash::FunctorSeq::onFinishSeq(FunctorSeq* subSeq)
{
	//���ܶ��߳�ͬʱ����
	
	if (subSeq && !subSeq->empty())
	{
		// ��subSeq��������_runningFunctor��������Ϊ���functor��ûrunning�ֱ꣬�����subseq���
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
	//���ܶ��߳̽���
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
				//���￪ʼ���߳�
				ScopeFlagAssert(_singleThreadVisitChecker_trySubmitNextFunctor);
				//��seq��ȫִ����ϣ����ϱ���
				if (_parent)
				{
					_parent->onSubSeqFinish();
				}
			}
			releaseRef();//�����ڴ�������֮�����ٷ���this
		}
	}
}

void Ash::FunctorSeq::doSubmitNextFunctor()
{
	ScopeFlagAssert(_singleThreadVisitChecker_doSubmitNextFunctor);
	TinyAssert(_runningFunctor == 0);
	TinyAssert(!empty());

	//�ҵ�Ҫ�ύ�ķ�Χ
	int start = _currentFunctor;
	int end = start;
	for (;end < static_cast<int>(_functors.size()) && _functors[end]; ++end){}
	TinyAssert(end > start);
	_currentFunctor = end;
	if(_currentFunctor < static_cast<int>(_functors.size()))
	{
		++_currentFunctor;
	}

	//�ύ����
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