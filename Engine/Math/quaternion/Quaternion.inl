//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Member Function
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
	const ValueType p = degreeToRadian(zRotate);
	const ValueType y = degreeToRadian(xRotate);
	const ValueType r = degreeToRadian(yRotate);

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
	const ValueType sin_2 = sin(degreeToRadian(angle / (ValueType)2));

	_w = cos(degreeToRadian(angle / (ValueType)2));
	_x = axis_norm.X() * sin_2;
	_y = axis_norm.Y() * sin_2;
	_z = axis_norm.Z() * sin_2;
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const VectorStorage<ValueType, 3>& from, const VectorStorage<ValueType, 3>& to)
{
	//http://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another

	ValueType k_cos_theta = from.dot(to);
	ValueType k = sqrt(from.lenth2() * to.lenth2());

	// 180 degree rotation around any orthogonal vector
	if (isEqual(k_cos_theta / k, (ValueType)-1))
	{
		VectorStorage<ValueType> orthogonal(1, 0, 0);
		orthogonal.crossInPlace(from);
		if (isEqual(orthogonal.lenth2(), (ValueType)0))
		{
			orthogonal = { 0,1,0 };
			orthogonal.crossInPlace(from);
		}
		orthogonal.normalizeInPlace();
		return Quaternion(0, orthogonal.X(), orthogonal.Y(), orthogonal.Z());
	}
	VectorStorage<ValueType, 3> cross = from.cross(to);
	return Quaternion(k_cos_theta + k, cross.X(), cross.Y(), cross.Z()).normalizeInPlace();
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
QuaternionStorage<ValueType>& QuaternionStorage<ValueType>::normalizeInPlace()
{
	ValueType mag = _x * _x + _y * _y + _z * _z + _w * _w;
	mag = sqrt(mag);

	_x /= mag;
	_y /= mag;
	_z /= mag;
	_w /= mag;
	return *this;
}
