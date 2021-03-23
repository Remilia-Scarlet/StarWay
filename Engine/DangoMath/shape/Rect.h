#pragma once
#include "DangoMath/MathDef.h"

namespace DangoMath
{
	template<class ValueType, int Size>
	class VectorStorage;

	template <class ValueType>
	class RectStorage
	{
	public:
		// init rect with x=y=w=h=0
		inline RectStorage();

		// init rect with x,y,w,h
		inline RectStorage(ValueType x, ValueType y, ValueType w, ValueType h);

		// return Vector2(x,y)
		inline VectorStorage<ValueType, 2> getLocation() const;

		// return Vector2(w,h)
		inline VectorStorage<ValueType, 2> getSize() const;

		// get / set x
		inline const ValueType& X() const;
		inline ValueType& X();

		// get / set y
		inline const ValueType& Y() const;
		inline ValueType& Y();

		// get / set width
		inline const ValueType& W() const;
		inline ValueType& W();

		// get / set height
		inline const ValueType& H() const;
		inline ValueType& H();

		// get left bound
		inline ValueType getLeft() const;

		// get top bound
		inline ValueType getTop() const;

		// get right bound
		inline ValueType getRight() const;

		// get bottom bound
		inline ValueType getBottom() const;

		// return if a point is contained in this rect
		inline bool contains(const VectorStorage<ValueType, 2>& point) const;
		inline bool contains(const ValueType& x, const ValueType& y) const;

		inline void reset();
		inline void reset(const ValueType& x, const ValueType& y, const ValueType& w, const ValueType& h);

		inline bool equal(const RectStorage<ValueType>& other) const;
	protected:

		ValueType _x;
		ValueType _y;
		ValueType _w;
		ValueType _h;
	};

	//////////////////////////////////////////////////////////////////////////
    // Global functions
    //////////////////////////////////////////////////////////////////////////
	template<class ValueType>
	inline bool operator==(const DangoMath::RectStorage<ValueType>& a, const DangoMath::RectStorage<ValueType>& b);
	//////////////////////////////////////////////////////////////////////////
	
	typedef RectStorage<float> Rect;
}

#include "Rect.inl"
