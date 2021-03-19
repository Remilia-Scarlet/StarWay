#pragma once
#include <functional>

namespace Ash
{
	class FunctorSeq;
	using Work = std::function<void(FunctorSeq&)>;

    class Functor
    {
		friend class FunctorSeq;
    public:
		Functor(Work work);

    protected:
		Work _work;
		FunctorSeq* _seq = nullptr;
    };

	class FunctorSeq
	{
	public:
		~FunctorSeq()
		{
			submit();
		}
		FunctorSeq& then(Functor&& functor);
		void submit();


		FunctorSeq& operator=(const FunctorSeq&) = delete;
		FunctorSeq& operator=(FunctorSeq&&);
	protected:
		FunctorSeq() = default;
		
		std::list<Functor> _functors;
		bool _submitted = false;
	};
}
