#include "Rect.h"
#include "Rect.h"
//////////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////////
template<class ValueType>
inline bool operator==(const RectStorage<ValueType>& a,const RectStorage<ValueType>& b)
{
	return a.equal(b);
}

//////////////////////////////////////////////////////////////////////////
// Member functions
//////////////////////////////////////////////////////////////////////////
template <class ValueType>
RectStorage<ValueType>::RectStorage()
{
	reset();
}

//template<class ValueType>
//inline RectStorage<ValueType>::RectStorage(const LuaVal & luaval)
//{
//	if (!luaval.isTable())
//	{
//		TinyAssert(false);
//		reset();
//		return;
//	}
//	float x = luaval.getField("x").convertFloat();
//	float y = luaval.getField("y").convertFloat();
//	float w = luaval.getField("w").convertFloat();
//	float h = luaval.getField("h").convertFloat();
//	reset(x, y, w, h);
//}


template <class ValueType>
RectStorage<ValueType>::RectStorage(ValueType x, ValueType y, ValueType w, ValueType h)
{
	reset(x, y, w, h);
}

template<class ValueType>
inline RectStorage<ValueType> RectStorage<ValueType>::createFromLua(lua_State * L, int index)
{
	int top = lua_gettop(L);
	int type = lua_type(L, index);
	if (type != LUA_TUSERDATA)
	{
		TinyAssert(false, "can't convert");
		return RectStorage<ValueType>();
	}
	if (lua_rawlen(L, index) != sizeof(RectStorage<ValueType>))
	{
		TinyAssert(false, "can't convert");
		return RectStorage<ValueType>();
	}
	TinyAssert(lua_gettop(L) == top);
	RectStorage<ValueType> ret;
	memcpy(&ret, lua_touserdata(L, index), sizeof(RectStorage<ValueType>));
	return ret;
}

template<class ValueType>
template<class ValT>
inline bool RectStorage<ValueType>::pushToLua(lua_State * L, const RectStorage<ValT>& val)
{
	void* data = lua_newuserdata(L, sizeof(RectStorage<ValT>));
	memcpy(data, &val, sizeof(RectStorage<ValT>));
	return true;
}

template <class ValueType>
VectorStorage<ValueType, 2> RectStorage<ValueType>::getLocation() const
{
	return VectorStorage<ValueType, 2>(_x, _y);
}

template <class ValueType>
VectorStorage<ValueType, 2> RectStorage<ValueType>::getSize() const
{
	return VectorStorage<ValueType, 2>(_w, _h);
}

template <class ValueType>
const ValueType& RectStorage<ValueType>::X() const
{
	return _x;
}

template <class ValueType>
ValueType& RectStorage<ValueType>::X()
{
	return _x;
}

template <class ValueType>
const ValueType& RectStorage<ValueType>::Y() const
{
	return _y;
}

template <class ValueType>
ValueType& RectStorage<ValueType>::Y()
{
	return _y;
}

template <class ValueType>
const ValueType& RectStorage<ValueType>::W() const
{
	return _w;
}

template <class ValueType>
ValueType& RectStorage<ValueType>::W()
{
	return _w;
}

template <class ValueType>
const ValueType& RectStorage<ValueType>::H() const
{
	return _h;
}

template <class ValueType>
ValueType& RectStorage<ValueType>::H()
{
	return _h;
}

template <class ValueType>
ValueType RectStorage<ValueType>::getLeft() const
{
	return _x;
}


template <class ValueType>
ValueType RectStorage<ValueType>::getTop() const
{
	return _y;
}

template <class ValueType>
ValueType RectStorage<ValueType>::getRight() const
{
	return _x + _w;
}

template <class ValueType>
ValueType RectStorage<ValueType>::getBottom() const
{
	return _y + _h;
}

template <class ValueType>
bool RectStorage<ValueType>::contains(const VectorStorage<ValueType, 2>& point) const
{
	return contains(point.X(), point.Y());
}

template <class ValueType>
bool RectStorage<ValueType>::contains(const ValueType& x, const ValueType& y) const
{
	return isGreaterOrEqual(x, _x) && isLess(x, _x + _w) && isGreaterOrEqual(y, _y) && isLess(y, _y + _h);
}

template <class ValueType>
void RectStorage<ValueType>::reset()
{
	_x = (ValueType)0;
	_y = (ValueType)0;
	_w = (ValueType)0;
	_h = (ValueType)0;
}

template <class ValueType>
void RectStorage<ValueType>::reset(const ValueType& x, const ValueType& y, const ValueType& w, const ValueType& h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

template <class ValueType>
bool RectStorage<ValueType>::equal(const RectStorage<ValueType>& other) const
{
	return isEqual(_x, other._x) && isEqual(_y, other._y) && isEqual(_w, other._w) && isEqual(_h, other._h);
}

