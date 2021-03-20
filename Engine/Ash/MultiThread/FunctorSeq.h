#pragma once
#include <atomic>
#include <functional>

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

namespace Ash
{
	class FunctorSeq;
	using Functor = std::function<void(FunctorSeq&)>;

	ASH_DEFINE_PTR(FunctorSeq);
	class FunctorSeq : public Ash::RefCountObj
	{
	public:
		static void entry(Functor functor);
		template<typename ...T>
		FunctorSeq& then(T ... functor);

	public:
		FunctorSeq& operator=(const FunctorSeq&) = delete;
	protected:
		void submit();
		void onFinishSeq(FunctorSeq* seq);
		void onSubSeqFinish();
		void trySubmitNextFunctor();
		void doSubmitNextFunctor();
		bool empty() const;
		template<typename ...T>
		void thenImpl(Functor functor, T ... rest);
		void thenImpl(Functor functor);
		FunctorSeq() = default;
		~FunctorSeq() override = default;
		
		std::vector<Functor> _functors;
		int _currentFunctor = 0;
		std::atomic_int _runningFunctor = 0;
		FunctorSeq* _parent = nullptr;



#ifdef TINY_DEBUG
		std::atomic_bool _singleThreadVisitChecker_trySubmitNextFunctor = false;
		std::atomic_bool _singleThreadVisitChecker_doSubmitNextFunctor = false;
#endif
	};

	template <typename... T>
	FunctorSeq& FunctorSeq::then(T... functor)
	{
		if(!_functors.empty())
			_functors.emplace_back(nullptr);
		thenImpl(std::forward<T>(functor)...);
		return *this;
	}

	template <typename ... T>
	void FunctorSeq::thenImpl(Functor functor, T... rest)
	{
		thenImpl(std::move(functor));
		thenImpl(std::forward<T>(rest)...);
	}

	

}
