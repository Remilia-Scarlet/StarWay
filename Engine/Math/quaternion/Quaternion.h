#pragma once
#include "Math\vector\Vector.h"
#include "Math\matrix\Matrix.h"

template <class ValueType>
class QuaternionStorage
{
public:
	// Init QuaternionStorage with w=1 and x=y=z=0
	inline QuaternionStorage();

	// Init QuaternionStorage with euler angles. In z,x,y(pitch,roll,yaw) order. eulerAngles.X() is x angel, eulerAngles.Y() is y angel, eulerAngles.Z() is z angel.
	inline QuaternionStorage(const VectorStorage<ValueType, 3>& eulerAngles);

	// Init QuaternionStorage with euler angles. In z,x,y(pitch,roll,yaw) order.
	inline QuaternionStorage(const ValueType& zRotate, const ValueType& xRotate, const ValueType& yRotate);

	// Init QuaternionStorage with a rotation by axis. The angle is degree.
	inline QuaternionStorage(const VectorStorage<ValueType, 3>& axis, const ValueType& angle);

	// Init QuaternionStorage with rotation from a vector to anoter vector
	inline QuaternionStorage(const VectorStorage<ValueType, 3>& from, const VectorStorage<ValueType, 3>& to);

	// Init QuaternionStorage with rotation matrix
	inline QuaternionStorage(const MatrixStorage<ValueType, 4, 4>& rotationMatrix);

	// Init QuaternionStorage with wxyz. Notice: you should be sure of the values' legality.
	inline QuaternionStorage(const ValueType& w, const ValueType& x, const ValueType& y, const ValueType& z);

	// Init QuaternionStorage with other QuaternionStorage
	inline QuaternionStorage(const QuaternionStorage& other);

	inline ~QuaternionStorage();

	// Init QuaternionStorage with w=1 and x=y=z=0
	inline void reset();

	// Init QuaternionStorage with euler angles. In z,x,y(pitch,roll,yaw) order. eulerAngles.X() is x angel, eulerAngles.Y() is y angel, eulerAngles.Z() is z angel.
	inline void reset(const Vector3& eulerAngles);

	// Init QuaternionStorage with euler angles. In z,x,y(pitch,roll,yaw) order.
	inline void reset(const ValueType& zRotate, const ValueType& xRotate, const ValueType& yRotate);

	// Init QuaternionStorage with a rotation by axis. The angle is degree.
	inline void reset(const VectorStorage<ValueType, 3>& axis, const ValueType& angle);

	// Init QuaternionStorage with rotation from a vector to anoter vector
	inline void reset(const VectorStorage<ValueType, 3>& from, const VectorStorage<ValueType, 3>& to);

	// Init QuaternionStorage with rotation matrix
	inline void reset(const MatrixStorage<ValueType, 4, 4>& rotationMatrix);

	// Init QuaternionStorage with wxyz. Notice: you should be sure of the values' legality.
	inline void reset(const ValueType& w, const ValueType& x, const ValueType& y, const ValueType& z);

	// Init QuaternionStorage with other QuaternionStorage
	inline void reset(const QuaternionStorage& other);


	ValueType& W();
	const ValueType& W() const;

	ValueType& X();
	const ValueType& X() const;

	ValueType& Y();
	const ValueType& Y() const;

	ValueType& Z();
	const ValueType& Z() const;

	// Copy this quaternion and normalize the copy. See also normalizeInPlace()
	QuaternionStorage normalized() const;

	// Normalize this quaternion and return this. See also normalized()
	QuaternionStorage& normalizeInPlace();
protected:
	ValueType _w;
	ValueType _x;
	ValueType _y;
	ValueType _z;
};

#include "Quaternion.inl"

typedef QuaternionStorage<float> Quaternion;