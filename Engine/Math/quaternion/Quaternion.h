#pragma once
#include "Math/MathDef.h"

template <class ValueType, int Size>
class VectorStorage;

template <class ValueType, int RowNum, int ColNum>
class MatrixStorage;

template <class ValueType>
class QuaternionStorage
{
public:
	// Init QuaternionStorage with w=1 and x=y=z=0
	inline QuaternionStorage();

	// Init QuaternionStorage with euler angles. In z,x,y(roll,pitch,yaw) order. eulerAngles.X() is x angel, eulerAngles.Y() is y angel, eulerAngles.Z() is z angel.
	inline QuaternionStorage(const VectorStorage<ValueType, 3>& eulerAngles);

	// Init QuaternionStorage with euler angles. In z,x,y(roll,pitch,yaw) order.
	inline QuaternionStorage(const ValueType& xRotate, const ValueType& yRotate, const ValueType& zRotate);

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

	// Init QuaternionStorage with euler angles. In z,x,y(roll,pitch,yaw) order. eulerAngles.X() is x angel, eulerAngles.Y() is y angel, eulerAngles.Z() is z angel.
	inline void reset(const VectorStorage<ValueType, 3>& eulerAngles);

	// Init QuaternionStorage with euler angles. In z,x,y(roll,pitch,yaw) order.
	inline void reset(const ValueType& xRotate, const ValueType& yRotate, const ValueType& zRotate);

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

	// Get an element by index. 0 is w, 1 is x, 2 is y, 3 is z
	inline ValueType& operator()(int index);
	inline const ValueType& operator()(int index) const;

	inline QuaternionStorage& operator=(const QuaternionStorage& other);

	inline ValueType& W();
	inline const ValueType& W() const;

	inline ValueType& X();
	inline const ValueType& X() const;

	inline ValueType& Y();
	inline const ValueType& Y() const;

	inline ValueType& Z();
	inline const ValueType& Z() const;

	// Copy this quaternion and normalize the copy. See also normalizeInPlace()
	inline QuaternionStorage normalized() const;

	// Normalize this quaternion and return this. See also normalized()
	inline QuaternionStorage& normalizeInPlace();

	// Is this quaternion normalized
	inline bool isNormalized() const;

	// Get rotation matrix4*4
	inline MatrixStorage<ValueType, 4, 4> toRotationMatrix()const;

	// Convert the rotation to euler angle, in order z,x,y(roll,pitch,yaw). The returned vector.X() is x angle, returned vector.Y() is y angle, returned vector.Z() is z angle.
	inline VectorStorage<ValueType, 3> toEularAngle() const;

	// Return a rotation axis and rotation angle, (return.first) is axis, (return.second) is angle.
	inline std::pair<VectorStorage<ValueType, 3>, ValueType> toAxisAngle() const;

	// Get a conjugate copy of this quaternion. See also conjugateInPlace()
	inline QuaternionStorage conjugate() const;

	// Make this quaternion conjugate and return self. See also conjugate()
	inline QuaternionStorage& conjugateInPlace();

	// Grassmann product. See also productInPlace().
	inline QuaternionStorage product(const QuaternionStorage<ValueType>& other) const;

	// Grassmann product. See also product()
	inline QuaternionStorage& productInPlace(const QuaternionStorage<ValueType>& other);

	// Check equal
	inline bool equal(const QuaternionStorage<ValueType>& other) const;
protected:
	ValueType _w;
	ValueType _x;
	ValueType _y;
	ValueType _z;
};

//////////////////////////////////////////////////////////////////////////
// Global Function
//////////////////////////////////////////////////////////////////////////
template<class ValueType>
bool operator==(const QuaternionStorage<ValueType>& a, const QuaternionStorage<ValueType>& b);

template<class ValueType>
QuaternionStorage<ValueType> operator*(const QuaternionStorage<ValueType>& a, const QuaternionStorage<ValueType>& b);
//////////////////////////////////////////////////////////////////////////


#include "Quaternion.inl"

typedef QuaternionStorage<float> Quaternion;