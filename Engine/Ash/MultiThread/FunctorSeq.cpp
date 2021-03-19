#include "FunctorSeq.h"

#include "Ash/MultiThread/ThreadPool.h"

Ash::Functor::Functor(Work work)
    :_work(std::move(work))
{
}

Ash::FunctorSeq& Ash::FunctorSeq::then(Functor&& functor)
{
	TinyAssert(!_submitted);
	functor._seq = this;
	_functors.emplace_back(std::move(functor));
	return *this;
}

void Ash::FunctorSeq::submit()
{
	TinyAssert(!_submitted);
	_submitted = true;
	auto functor = std::move(_functors.front());
	_functors.pop_front();
	ThreadPool::instance()->dispatchFunctor(std::move(functor));
}
