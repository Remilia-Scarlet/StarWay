//////////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////////
template<class ValueType>
inline bool DangoMath::operator==(const DangoMath::RectStorage<ValueType>& a,const DangoMath::RectStorage<ValueType>& b)
{
	return a.equal(b);
}

//////////////////////////////////////////////////////////////////////////
// Member functions
//////////////////////////////////////////////////////////////////////////
template <class ValueType>
DangoMath::RectStorage<ValueType>::RectStorage()
{
	reset();
}


template <class ValueType>
DangoMath::RectStorage<ValueType>::RectStorage(ValueType x, ValueType y, ValueType w, ValueType h)
{
	reset(x, y, w, h);
}

template <class ValueType>
DangoMath::VectorStorage<ValueType, 2> DangoMath::RectStorage<ValueType>::getLocation() const
{
	return VectorStorage<ValueType, 2>(_x, _y);
}

template <class ValueType>
DangoMath::VectorStorage<ValueType, 2> DangoMath::RectStorage<ValueType>::getSize() const
{
	return VectorStorage<ValueType, 2>(_w, _h);
}

template <class ValueType>
const ValueType& DangoMath::RectStorage<ValueType>::X() const
{
	return _x;
}

template <class ValueType>
ValueType& DangoMath::RectStorage<ValueType>::X()
{
	return _x;
}

template <class ValueType>
const ValueType& DangoMath::RectStorage<ValueType>::Y() const
{
	return _y;
}

template <class ValueType>
ValueType& DangoMath::RectStorage<ValueType>::Y()
{
	return _y;
}

template <class ValueType>
const ValueType& DangoMath::RectStorage<ValueType>::W() const
{
	return _w;
}

template <class ValueType>
ValueType& DangoMath::RectStorage<ValueType>::W()
{
	return _w;
}

template <class ValueType>
const ValueType& DangoMath::RectStorage<ValueType>::H() const
{
	return _h;
}

template <class ValueType>
ValueType& DangoMath::RectStorage<ValueType>::H()
{
	return _h;
}

template <class ValueType>
ValueType DangoMath::RectStorage<ValueType>::getLeft() const
{
	return _x;
}


template <class ValueType>
ValueType DangoMath::RectStorage<ValueType>::getTop() const
{
	return _y;
}

template <class ValueType>
ValueType DangoMath::RectStorage<ValueType>::getRight() const
{
	return _x + _w;
}

template <class ValueType>
ValueType DangoMath::RectStorage<ValueType>::getBottom() const
{
	return _y + _h;
}

template <class ValueType>
bool DangoMath::RectStorage<ValueType>::contains(const VectorStorage<ValueType, 2>& point) const
{
	return contains(point.X(), point.Y());
}

template <class ValueType>
bool DangoMath::RectStorage<ValueType>::contains(const ValueType& x, const ValueType& y) const
{
	return isGreaterOrEqual(x, _x) && isLess(x, _x + _w) && isGreaterOrEqual(y, _y) && isLess(y, _y + _h);
}

template <class ValueType>
void DangoMath::RectStorage<ValueType>::reset()
{
	_x = (ValueType)0;
	_y = (ValueType)0;
	_w = (ValueType)0;
	_h = (ValueType)0;
}

template <class ValueType>
void DangoMath::RectStorage<ValueType>::reset(const ValueType& x, const ValueType& y, const ValueType& w, const ValueType& h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

template <class ValueType>
bool DangoMath::RectStorage<ValueType>::equal(const RectStorage<ValueType>& other) const
{
	return isEqual(_x, other._x) && isEqual(_y, other._y) && isEqual(_w, other._w) && isEqual(_h, other._h);
}

