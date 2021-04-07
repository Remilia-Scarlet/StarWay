#pragma once
#include <atomic>
#include <functional>
#include <variant>

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/MultiThread/SpinMutex.h"

namespace Ash
{
	class FunctorSeq;
	using Functor = std::function<void(FunctorSeq&)>;
	class Future
	{
		friend class FunctorSeq;
	private:
		Future(RefCountPtr<FunctorSeq> seq) :_seq(std::move(seq)) {}
		RefCountPtr<FunctorSeq> _seq;
	};
	struct FunctorSaving
	{
		enum class FunctorType : uint8_t
		{
			ThenFunctor,
			FutureFunctor,
			Future,
			LoopFunctor,
			Separator
		};
		struct ThenFactorStruct
		{
			Functor _functor;
		};
		struct FutureFunctorStruct
		{
			Functor _functor;
			RefCountPtr<FunctorSeq> _seq;
		};
		struct FutureStruct
		{
			FunctorSeq* _seq;
		};
		struct LoopFunctorStruct
		{
			Functor _functor;
			std::function<bool()> _pred;
		};
		struct SeparatorStruct
		{
		};
		FunctorSaving(FunctorType type, std::variant<ThenFactorStruct, FutureFunctorStruct, FutureStruct, LoopFunctorStruct, SeparatorStruct> data)
			:_functorType(type), _data(std::move(data))
		{
			TinyAssert(_data.index() == static_cast<size_t>(_functorType));
		}
		FunctorType _functorType;
		std::variant<ThenFactorStruct, FutureFunctorStruct, FutureStruct, LoopFunctorStruct, SeparatorStruct> _data;
	};


    //Usage:
	//Ash::FunctorSeq::entry([](Ash::FunctorSeq& seq)
	//{
	//	DebugString("A");
    //
	//  auto future = seq.future([](Ash::FunctorSeq& seq)
	//  {
	//     DebugString("C");
	//  });
	//	seq.then([](Ash::FunctorSeq& seq)
	//	{
	//		DebugString("D");
	//		seq.then([](Ash::FunctorSeq&)
	//		{
	//			DebugString("F");
	//		});
	//	}, [](Ash::FunctorSeq&)
	//	{
	//		DebugString("E");
	//	});
    //
    //  seq.then(future);
	//
	//	seq.then([](Ash::FunctorSeq&)
	//	{
	//		DebugString("G");
	//	});
	//
	//  seq.loop([int i = 3]()
	//  {
	//     return --i >= 0;
	//  }, [](Ash::FunctorSeq&)
	//  {
	//      DebugString("H");
	//  }); 
	//
	//  DebugString("B");
	//});
    //A prints first.
    //B prints next. (After the whole recording finishing, sequences start to execute)
    //C prints after B, and before G. (After calling seq.future(foo), the foo can start. Before G, we use seq.then(future) to ensure future is done.
    //D prints before F.
    //C, D, E, F all print before G
    //The last, print H for 3 times.
	class FunctorSeq : Ash::RefCountObj
	{
	public:
		static void entry(const Functor& functor);
		
		template<typename ...Functors>
		FunctorSeq& then(Functors ... functor);
		FunctorSeq& then(std::vector<Functor> functors);

		Future future(Functor functor);

		FunctorSeq& loop(std::function<bool()> pred, Functor functor);

		void setDebugName(std::string debugName);
	public:
		FunctorSeq& operator=(const FunctorSeq&) = delete;
	protected:
		void submit();
		void onFinishFunctor(FunctorSeq* newRecordedSeq, const FunctorSaving& theFinishedFunctor);
		void onSubSeqFinish();
		void submitNextFunctor(bool isInitialSubmit);
		void doSubmitNextFunctor();
		bool empty() const;
		template<typename ...T>
		void thenImpl(Functor functor, T ... rest);
		template<typename ...T>
		void thenImpl(Future future, T...rest);
		void thenImpl(Functor functor);
		void thenImpl(Future future);
		FunctorSeq() = default;
		~FunctorSeq() = default;

		void pushSeparatorFunctor();
		void pushFutureFunctor(Functor functor, FunctorSeq* seq);
		void pushFuture(Future future);
		void pushThenFunctor(Functor functor);
		void pushLoopFunctor(Functor functor, std::function<bool()> pred);

		std::vector<FunctorSaving> _functors;
		int _currentFunctor = 0;
		std::atomic_int _runningFunctor = 0;
		FunctorSeq* _parent = nullptr;
		SpinMutex _futureMutex;
		bool _futureHasFinished = false;
		void runFunctor(const FunctorSaving& functor);


#ifdef TINY_DEBUG
		std::string _debugName;
		std::atomic_bool _singleThreadVisitChecker_trySubmitNextFunctor = false;
		std::atomic_bool _singleThreadVisitChecker_doSubmitNextFunctor = false;
#endif
	};

	template <typename... T>
	FunctorSeq& FunctorSeq::then(T... functor)
	{
		if (!_functors.empty())
			pushSeparatorFunctor();
		thenImpl(std::forward<T>(functor)...);
		return *this;
	}

	template <typename ... T>
	void FunctorSeq::thenImpl(Functor functor, T... rest)
	{
		thenImpl(std::move(functor));
		thenImpl(std::forward<T>(rest)...);
	}

	template <typename ... T>
	void FunctorSeq::thenImpl(Future future, T... rest)
	{
		thenImpl(std::move(future));
		thenImpl(std::forward<T>(rest)...);
	}
}
