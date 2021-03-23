#pragma once
#include <atomic>
#include <functional>

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

namespace Ash
{
	class FunctorSeq;
	using Functor = std::function<void(FunctorSeq&)>;

	class FunctorSeq
	{
	public:
		static void entry(const Functor& functor);
		template<typename ...T>
		FunctorSeq& then(T ... functor);

		void setDebugName(std::string debugName);
	public:
		FunctorSeq& operator=(const FunctorSeq&) = delete;
	protected:
		void submit();
		void onFinishFunctor(FunctorSeq* seq);
		void onSubSeqFinish();
		void submitNextFunctor(bool isInitialSubmit);
		void doSubmitNextFunctor();
		bool empty() const;
		template<typename ...T>
		void thenImpl(Functor functor, T ... rest);
		void thenImpl(Functor functor);
		static void runFunctor(const Functor& functor, FunctorSeq* parent);
		FunctorSeq() = default;
		~FunctorSeq() = default;
		
		std::vector<Functor> _functors;
		int _currentFunctor = 0;
		std::atomic_int _runningFunctor = 0;
		FunctorSeq* _parent = nullptr;



#ifdef TINY_DEBUG
		std::string _debugName;
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
