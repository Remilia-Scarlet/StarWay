#pragma once
#include <atomic>
#include <functional>
#include <variant>

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

namespace Ash
{
	class FunctorSeq;
	using Functor = std::function<void(FunctorSeq&)>;
	class Future
	{
	private:
		FunctorSeq* _seq = nullptr;
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
			FunctorSeq* _seq;
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
			:_functorType(type)
		{
			switch (_functorType)
			{
			case FunctorType::ThenFunctor:
			{
				ThenFactorStruct& strc = std::get<ThenFactorStruct>(data);
				_data = std::move(strc);
				break;
			}
			case FunctorType::FutureFunctor:
			{
				FutureFunctorStruct& strc = std::get<FutureFunctorStruct>(data);
				_data = std::move(strc);
				break;
			}
			case FunctorType::Future: 
			{
				FutureStruct& strc = std::get<FutureStruct>(data);
				_data = std::move(strc);
				break;
			}
			case FunctorType::LoopFunctor: 
			{
				LoopFunctorStruct& strc = std::get<LoopFunctorStruct>(data);
				_data = std::move(strc);
				break;
			}
			case FunctorType::Separator:
			{
				SeparatorStruct& strc = std::get<SeparatorStruct>(data);
				_data = std::move(strc);
				break;
			}
			}
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
	class FunctorSeq
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
		void onFinishFunctor(FunctorSeq* seq);
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
		void pushFutureFunctor(Functor functor);
		void pushFuture(Future future);
		void pushThenFunctor(Functor functor);
		void pushLoopFunctor(Functor functor, std::function<bool()> pred);

		std::vector<FunctorSaving> _functors;
		int _currentFunctor = 0;
		std::atomic_int _runningFunctor = 0;
		FunctorSeq* _parent = nullptr;
		static void runFunctor(const FunctorSaving& functor, FunctorSeq* parent);


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
