//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////////////////////////////
//Member Function
//////////////////////////////////////////////////////////////////////////
template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage()
{
	reset();
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const VectorStorage<ValueType, 3>& eulerAngles)
{
	reset(eulerAngles);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const ValueType& zRotate, const ValueType& xRotate, const ValueType& yRotate)
{
	reset(zRotate, xRotate, yRotate);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const VectorStorage<ValueType, 3>& axis, const ValueType& angle)
{
	reset(axis, angle);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const VectorStorage<ValueType, 3>& from, const VectorStorage<ValueType, 3>& to)
{
	reset(from, to);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const MatrixStorage<ValueType, 4, 4>& rotationMatrix)
{
	reset(rotationMatrix);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const ValueType& w, const ValueType& x, const ValueType& y, const ValueType& z)
{
	reset(w, x, y, z);
}

template <class ValueType>
QuaternionStorage<ValueType>::QuaternionStorage(const QuaternionStorage& other)
{
	reset(other);
}

template <class ValueType>
QuaternionStorage<ValueType>::~QuaternionStorage()
{

}

template <class ValueType>
void QuaternionStorage<ValueType>::reset()
{
	_w = 1;
	_x = 0;
	_y = 0;
	_z = 0;
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const Vector3& eulerAngles)
{
	reset(eulerAngles.Z(), eulerAngles.X(), eulerAngles.Y());
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const ValueType& zRotate, const ValueType& xRotate, const ValueType& yRotate)
{
	const ValueType p = (ValueType)M_PI / (ValueType)360.0 * zRotate;
	const ValueType y = (ValueType)M_PI / (ValueType)360.0 * xRotate;
	const ValueType r = (ValueType)M_PI / (ValueType)360.0 * yRotate;

	const ValueType sinp = sin(p);
	const ValueType siny = sin(y);
	const ValueType sinr = sin(r);
	const ValueType cosp = cos(p);
	const ValueType cosy = cos(y);
	ValueType cosr = cos(r);

	_w = cosr * cosp * cosy + sinr * sinp * siny;
	_x = sinr * cosp * cosy - cosr * sinp * siny;
	_y = cosr * sinp * cosy + sinr * cosp * siny;
	_z = cosr * cosp * siny - sinr * sinp * cosy;
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const VectorStorage<ValueType, 3>& axis, const ValueType& angle)
{
	VectorStorage<ValueType, 3> axis_norm = axis.normalized();
	const ValueType sin_2 = sin(angle / (ValueType)2);

	_w = cos(angle / (ValueType)2);
	_x = axis_norm.X() * sin_2;
	_y = axis_norm.Y() * sin_2;
	_z = axis_norm.Z() * sin_2;
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const VectorStorage<ValueType, 3>& from, const VectorStorage<ValueType, 3>& to)
{
	VectorStorage<ValueType, 3> from_norm = from.normalized();
	VectorStorage<ValueType, 3> to_norm = to.normalized();

	ValueType d = from_norm * to_norm;

	constexpr bool isFloat = std::is_floating_point<ValueType>::value;
	if ((isFloat && TINY_FLOAT_EQUAL(d, (ValueType)1)) //same directions
		|| (!isFloat && d == (ValueType)1))
	{
		return QuaternionStorage<ValueType>();
	}
	else if (isFloat && TINY_FLOAT_EQUAL(d, (ValueType)(-1))  //opposite directions
		|| (!isFloat && d == (ValueType)(-1))
		)
	{
		VectorStorage<ValueType> ax(1, 0, 0);
		ax.cross(from_norm);
		if (ax.dot(ax) == 0) {
			ax = cv::Vec3d(0, 1, 0);
			ax = ax.cross(from_n);
		}
		QuaternionStorage qr(ax,0);
		return qr;
	}

	double s = (double)sqrt((1 + d) * 2);
	double inv_s = 1.0f / s;

	cv::Vec3d axis = from_n.cross(to_n);
	Quaternion q(s*0.5f, axis[0] * inv_s, axis[1] * inv_s, axis[2] * inv_s);
	return q.Normalize();
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const MatrixStorage<ValueType, 4, 4>& rotationMatrix)
{

}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const ValueType& w, const ValueType& x, const ValueType& y, const ValueType& z)
{

}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const QuaternionStorage& other)
{

}

template <class ValueType>
ValueType& QuaternionStorage<ValueType>::W()
{
	return _w;
}

template <class ValueType>
const ValueType& QuaternionStorage<ValueType>::W() const
{
	return _w;
}

template <class ValueType>
ValueType& QuaternionStorage<ValueType>::X()
{
	return _x;
}

template <class ValueType>
const ValueType& QuaternionStorage<ValueType>::X() const
{
	return _x;
}

template <class ValueType>
ValueType& QuaternionStorage<ValueType>::Y()
{
	return _y;
}

template <class ValueType>
const ValueType& QuaternionStorage<ValueType>::Y() const
{
	return _y;
}

template <class ValueType>
ValueType& QuaternionStorage<ValueType>::Z()
{
	return _z;
}

template <class ValueType>
const ValueType& QuaternionStorage<ValueType>::Z() const
{
	return _z;
}

template <class ValueType>
QuaternionStorage<ValueType> QuaternionStorage<ValueType>::normalized() const
{
	QuaternionStorage<ValueType> ret = *this;
	return ret.normalizeInPlace();
}

template <class ValueType>
QuaternionStorage& QuaternionStorage<ValueType>::normalizeInPlace()
{
	ValueType mag = _x * _x + _y * _y + _z * _z + _w * _w;
	mag = sqrt(mag);

	_x /= mag;
	_y /= mag;
	_z /= mag;
	_w /= mag;
	return *this;
}
