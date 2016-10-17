//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
template<class ValueType>
bool operator==(const QuaternionStorage<ValueType>& a, const QuaternionStorage<ValueType>& b)
{
	return isEqual(a.X(), b.X())
		&& isEqual(a.Y(), b.Y())
		&& isEqual(a.Z(), b.Z())
		&& isEqual(a.W(), b.W());
}

template<class ValueType>
QuaternionStorage<ValueType> operator*(const QuaternionStorage<ValueType>& a, const QuaternionStorage<ValueType>& b)
{
	return a.product(b);
}
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
QuaternionStorage<ValueType>::QuaternionStorage(const ValueType& xRotate, const ValueType& yRotate, const ValueType& zRotate)
{
	reset(xRotate, yRotate, zRotate);
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
void QuaternionStorage<ValueType>::reset(const VectorStorage<ValueType,3>& eulerAngles)
{
	reset(eulerAngles.X(), eulerAngles.Y(), eulerAngles.Z());
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const ValueType& xRotate, const ValueType& yRotate, const ValueType& zRotate)
{
	// Copy from DirectX::XMQuaternionRotationRollPitchYawFromVector
	const ValueType r = degToRad(zRotate) / (ValueType)2;
	const ValueType p = degToRad(xRotate) / (ValueType)2;
	const ValueType y = degToRad(yRotate) / (ValueType)2;

	const ValueType sinr = sin(r);
	const ValueType sinp = sin(p);
	const ValueType siny = sin(y);
	const ValueType cosr = cos(r);
	const ValueType cosp = cos(p);
	const ValueType cosy = cos(y);

	VectorStorage<ValueType, 4> p0 = { sinp, cosp, cosp, cosp };
	VectorStorage<ValueType, 4> y0 = { cosy, siny, cosy, cosy };
	VectorStorage<ValueType, 4> r0 = { cosr, cosr, sinr, cosr };
	VectorStorage<ValueType, 4> p1 = { cosp, sinp, sinp, sinp };
	VectorStorage<ValueType, 4> y1 = { siny, cosy, siny, siny };
	VectorStorage<ValueType, 4> r1 = { sinr, sinr, cosr, sinr };

	static const VectorStorage<ValueType, 4>  sign = { (ValueType)1.0f, (ValueType)-1.0f, (ValueType)-1.0f, (ValueType)1.0f };

	VectorStorage<ValueType, 4> q0 = p0.multiply(y0);
	VectorStorage<ValueType, 4> q1 = sign.multiply(p1);

	q0.multiplyInPlace(r0);
	q1.multiplyInPlace(y1);

	VectorStorage<ValueType, 4> Q = q1.multiply(r1) + q0;
	_x = Q.X();
	_y = Q.Y();
	_z = Q.Z();
	_w = Q.W();
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const VectorStorage<ValueType, 3>& axis, const ValueType& angle)
{
	VectorStorage<ValueType, 3> axis_norm = axis.normalized();
	const ValueType sin_2 = sin(degToRad(angle / (ValueType)2));

	_w = cos(degToRad(angle / (ValueType)2));
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
		VectorStorage<ValueType, 3> orthogonal{ 1, 0, 0 };
		orthogonal.crossInPlace(from);
		if (isEqual(orthogonal.lenth2(), (ValueType)0))
		{
			orthogonal = { 0,1,0 };
			orthogonal.crossInPlace(from);
		}
		orthogonal.normalizeInPlace();

		_w = 0;
		_x = orthogonal.X();
		_y = orthogonal.Y();
		_z = orthogonal.Z();
		return;
	}
	VectorStorage<ValueType, 3> cross = from.cross(to);

	_w = k_cos_theta + k;
	_x = cross.X();
	_y = cross.Y();
	_z = cross.Z();
	normalizeInPlace();
}

template <class ValueType>
void QuaternionStorage<ValueType>::reset(const MatrixStorage<ValueType, 4, 4>& rotationMatrix)
{
	// I don't know how to build quaternion by matrix, so I copied this function from DirextX::XMQuaternionRotationMatrix

	ValueType r22 = rotationMatrix(2, 2);
	if (r22 <= 0.f)  // x^2 + y^2 >= z^2 + w^2
	{
		ValueType dif10 = rotationMatrix(1,1) - rotationMatrix(0,0);
		ValueType omr22 = (ValueType)1 - r22;
		if (isLess(dif10, (ValueType)0))  // x^2 >= y^2
		{
			ValueType fourXSqr = omr22 - dif10;
			ValueType inv4x = (ValueType)0.5 / sqrtf(fourXSqr);
			_x = fourXSqr*inv4x;
			_y = (rotationMatrix(0,1) + rotationMatrix(1,0))*inv4x;
			_z = (rotationMatrix(0,2) + rotationMatrix(2,0))*inv4x;
			_w = (rotationMatrix(1,2) - rotationMatrix(2,1))*inv4x;
		}
		else  // y^2 >= x^2
		{
			ValueType fourYSqr = omr22 + dif10;
			ValueType inv4y = (ValueType)0.5 / sqrtf(fourYSqr);
			_x = (rotationMatrix(0,1) + rotationMatrix(1,0))*inv4y;
			_y = fourYSqr*inv4y;
			_z = (rotationMatrix(1,2) + rotationMatrix(2,1))*inv4y;
			_w = (rotationMatrix(2,0) - rotationMatrix(0,2))*inv4y;
		}
	}
	else  // z^2 + w^2 >= x^2 + y^2
	{
		ValueType sum10 = rotationMatrix(1,1) + rotationMatrix(0,0);
		ValueType opr22 = (ValueType)1 + r22;
		if (isLess(sum10 , (ValueType)0))  // z^2 >= w^2
		{
			ValueType fourZSqr = opr22 - sum10;
			ValueType inv4z = (ValueType)0.5 / sqrtf(fourZSqr);
			_x = (rotationMatrix(0,2) + rotationMatrix(2,0))*inv4z;
			_y = (rotationMatrix(1,2) + rotationMatrix(2,1))*inv4z;
			_z = fourZSqr*inv4z;
			_w = (rotationMatrix(0,1) - rotationMatrix(1,0))*inv4z;
		}
		else  // w^2 >= z^2
		{
			ValueType fourWSqr = opr22 + sum10;
			ValueType inv4w = (ValueType)0.5 / sqrtf(fourWSqr);
			_x = (rotationMatrix(1,2) - rotationMatrix(2,1))*inv4w;
			_y = (rotationMatrix(2,0) - rotationMatrix(0,2))*inv4w;
			_z = (rotationMatrix(0,1) - rotationMatrix(1,0))*inv4w;
			_w = fourWSqr*inv4w;
		}
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
	static ValueType forWrong = 0;
	return forWrong;
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
	return *this;
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
bool QuaternionStorage<ValueType>::isNormalized() const
{
	return isEqual(_x*_x + _y*_y + _z*_z + _w*_w, (ValueType)1);
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

template <class ValueType>
VectorStorage<ValueType, 3> QuaternionStorage<ValueType>::toEularAngle() const
{
	return VectorStorage<ValueType, 3>{
		radToDeg(asin((ValueType)2 * (_w * _x - _y * _z))),															// x(pitch)
		radToDeg(atan2((ValueType)2 * (_w * _y + _x * _z), (ValueType)1 - (ValueType)2 * (_x * _x + _y * _y))),		// y(yaw)
		radToDeg(atan2((ValueType)2 * (_w * _z + _x * _y), (ValueType)1 - (ValueType)2 * (_x * _x + _z * _z))),		// z(roll)
	};
}

template <class ValueType>
std::pair<VectorStorage<ValueType, 3>, ValueType> QuaternionStorage<ValueType>::toAxisAngle() const
{
	ValueType theta_2 = acos(_w);
	ValueType sin_theta_2 = sin(theta_2);
	if (isEqual(sin_theta_2, (ValueType)0))
		return std::make_pair(VectorStorage<ValueType, 3>(), (ValueType)0);
	return std::make_pair(
		VectorStorage<ValueType, 3>{_x / sin_theta_2, _y / sin_theta_2, _z / sin_theta_2},
		radToDeg(theta_2 * (ValueType)2)
		);
}

template <class ValueType>
QuaternionStorage<ValueType> QuaternionStorage<ValueType>::conjugate() const
{
	QuaternionStorage<ValueType> val = *this;
	return val.conjugateInPlace();
}

template <class ValueType>
QuaternionStorage<ValueType>& QuaternionStorage<ValueType>::conjugateInPlace()
{
	_x = -_x;
	_y = -_y;
	_z = -_z;
	return *this;
}

template <class ValueType>
QuaternionStorage<ValueType> QuaternionStorage<ValueType>::product(const QuaternionStorage<ValueType>& other) const
{
	const QuaternionStorage<ValueType>& p = *this;
	const QuaternionStorage<ValueType>& q = other;

	return QuaternionStorage(
		p._w * q._w - p._x * q._x - p._y * q._y - p._z * q._z,
		p._w * q._x + p._x * q._w + p._y * q._z - p._z * q._y,
		p._w * q._y + p._y * q._w + p._z * q._x - p._x * q._z,
		p._w * q._z + p._z * q._w + p._x * q._y - p._y * q._x
		);
}

template <class ValueType>
QuaternionStorage<ValueType>& QuaternionStorage<ValueType>::productInPlace(const QuaternionStorage<ValueType>& other)
{
	*this = product(other);
	return *this;
}
