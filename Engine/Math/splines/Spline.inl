#include "Spline.h"
//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Member Function
//////////////////////////////////////////////////////////////////////////

template<class ValueType, int VecSize>
inline SplineStorage<ValueType, VecSize>::SplineStorage(const std::vector<SplinePoint>& controlPoints)
{
}

template<class ValueType, int VecSize>
inline SplinePoint SplineStorage<ValueType, VecSize>::get(float t) const
{
	TinyAssert(t >= 0.f && t <= 1.f);

}

template<class ValueType, int VecSize>
inline SplinePoint SplineStorage<ValueType, VecSize>::getTangent(int i)
{
	TinyAssert(i >= 0 && i < (int)_controlPoints.size());
	if (_controlPoints.size() == 1)
		return _controlPoints[0];

	if (i == 0)
	{
		return _controlPoints[1] - _controlPoints[0] / (ValueType(1)/ValueType(_controlPoints.size() - 1))
	}
	else if (i == (int)_controlPoints.size() - 1)
	{
		return _controlPoints[_controlPoints.size() - 1] - _controlPoints[_controlPoints.size() - 2] / (ValueType(1) / ValueType(_controlPoints.size() - 1))
	}
	else
	{
		return _controlPoints[i + 1] - _controlPoints[i - 1] / (ValueType(2) / ValueType(_controlPoints.size() - 1))
	}
}
