#pragma once
#include "Math/MathDef.h"

template <class ValueType, int Size>
class VectorStorage;

template<class ValueType, int VecSize>
class SplineStorage
{
public:
	typedef VectorStorage<ValueType, VecSize> SplinePoint;
public:
	inline SplineStorage(const std::vector<SplinePoint>& controlPoints);

	// get value at t; t must between [0,1]
	inline SplinePoint get(float t) const;
protected:
	// get tangent of point i; i is the index of control point
	inline SplinePoint getTangent(int i);

	std::vector<SplinePoint> _controlPoints
};

//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#include "Spline.inl"

typedef SplineStorage<float,3> Spline;
