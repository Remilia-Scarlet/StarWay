#pragma once
#include <atomic>
#include <functional>

#include "Ash/RefCountPointer/RefCountObj.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

namespace Ash
{
	class FunctorSeq;
	using Functor = std::function<void(FunctorSeq&)>;
    class Future : public Functor
    {
        
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

		template<typename Functor>
		Future future(Functor functor);

		template<typename Pred, typename Functor>
		FunctorSeq& loop(Pred pred, Functor functor);

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
