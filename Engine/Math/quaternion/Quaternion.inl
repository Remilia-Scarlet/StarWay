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
	ValueType trace = rotationMatrix(0, 0) + rotationMatrix(1, 1) + rotationMatrix(2, 2);
	if (isGreater(trace,(ValueType)0))
	{
		ValueType s = sqrt(trace + (ValueType)1);
		ValueType t = s * (ValueType)0.5;
		X() = (rotationMatrix(2, 1) - rotationMatrix(1, 2)) * t;
		Y() = (rotationMatrix(0, 2) - rotationMatrix(2, 0)) * t;
		Z() = (rotationMatrix(1, 0) - rotationMatrix(0, 1)) * t;
		W() = s * (ValueType)0.5;
	}
	else
	{
		int i = 0;
		if (rotationMatrix(1, 1) > rotationMatrix(0, 0))
			i = 1;
		if (rotationMatrix(2, 2) > rotationMatrix(i, i))
			i = 2;

		const static int next[3] = { 1,2,0 };
		int j = next[i];
		int k = next[j];

		ValueType s = sqrt(rotationMatrix(i, i) - (rotationMatrix(j, j) + rotationMatrix(k, k)) + (ValueType)1);
		ValueType t;
		if (!isEqual(s, (ValueType)0))
			t = (ValueType)0.5 / s;
		else
			t = s;

		(*this)(i + 1) = s * (ValueType)0.5;
		(*this)(j + 1) = (rotationMatrix(j, i) + rotationMatrix(i, j))*t;
		(*this)(k + 1) = (rotationMatrix(k, i) + rotationMatrix(i, k))*t;
		(*this)(0) = (rotationMatrix(k, j) + rotationMatrix(j, k))*t;
	}
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const ValueType& w, const ValueType& x, const ValueType& y, const ValueType& z)
{
	_w = w;
	_x = x;
	_y = y;
	_z = z;
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const QuaternionStorage& other)
{
	reset(other.W(), other.X(), other.Y(), other.Z());
}

template <class ValueType>
ValueType& QuaternionStorage<ValueType>::operator()(int index)
{
	switch (index)
	{
	case 0:
		return _w;
	case 1:
		return _x;
	case 2:
		return _y;
	case 3:
		return _z;
	}
	TinyAssert(false,"Quaternion operator() index wrong");
	return 0;
}

template <class ValueType>
const ValueType& QuaternionStorage<ValueType>::operator()(int index) const
{
	return const_cast<QuaternionStorage<ValueType>&>(*this)(index);
}

template <class ValueType>
QuaternionStorage<ValueType>& QuaternionStorage<ValueType>::operator=(const QuaternionStorage& other)
{
	reset(other);
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

template <class ValueType>
MatrixStorage<ValueType, 4, 4> QuaternionStorage<ValueType>::toRotationMatrix() const
{
	MatrixStorage<ValueType, 4, 4> matrix = {
		1 - 2 * Y()*Y() - 2 * Z()*Z(),	2 * X()*Y() + 2 * Z()*W(),		2 * X()*Y() - 2 * Y()*W(),		0 ,
		2 * X()*Y() - 2 * Z()*W(),		1 - 2 * X()*X() - 2 * Z()*Z(),	2 * Y()*Z() + 2 * X()*W(),		0 ,
		2 * X()*Z() + 2 * Y()*W(),		2 * Y()*Z() - 2 * X()*W(),		1 - 2 * X()*X() - 2 * Y()*Y(),	0 ,
		0,								0,								0,								1
	};
	return matrix;
}
