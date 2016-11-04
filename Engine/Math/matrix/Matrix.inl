//////////////////////////////////////////////////////////////////////////
// Global function
//////////////////////////////////////////////////////////////////////////

// create a rotation matrix rotating by x axis. 
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateRotationMatrixX(const ValueType& angle)
{
	ValueType sinAngle = sin(degToRad(angle));
	ValueType cosAngle = cos(degToRad(angle));
	
	return MatrixStorage<ValueType, 4, 4>{
		(ValueType)1,	(ValueType)0,	(ValueType)0,	(ValueType)0,
		(ValueType)0,	cosAngle,		sinAngle,		(ValueType)0,
		(ValueType)0,	-sinAngle,		cosAngle,		(ValueType)0,
		(ValueType)0,	(ValueType)0,	(ValueType)0,	(ValueType)1
	};
}

// create a rotation matrix rotating by y axis. 
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateRotationMatrixY(const ValueType& angle)
{
	ValueType sinAngle = sin(degToRad(angle));
	ValueType cosAngle = cos(degToRad(angle));

	return MatrixStorage<ValueType, 4, 4>{
		cosAngle,		(ValueType)0,	-sinAngle,		(ValueType)0,
		(ValueType)0,	(ValueType)1,	(ValueType)0,	(ValueType)0,
		sinAngle,		(ValueType)0,	cosAngle,		(ValueType)0,
		(ValueType)0,	(ValueType)0,	(ValueType)0,	(ValueType)1
	};
}

// create a rotation matrix rotating by z axis. 
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateRotationMatrixZ(const ValueType& angle)
{
	ValueType sinAngle = sin(degToRad(angle));
	ValueType cosAngle = cos(degToRad(angle));

	return MatrixStorage<ValueType, 4, 4>{
		cosAngle,		sinAngle,		(ValueType)0,	(ValueType)0,
		-sinAngle,		cosAngle,		(ValueType)0,	(ValueType)0,
		(ValueType)0,	(ValueType)0,	(ValueType)1,	(ValueType)0,
		(ValueType)0,	(ValueType)0,	(ValueType)0,	(ValueType)1
	};
}

// create a translation matrix
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateTranslaionMatrix(const ValueType& x, const ValueType& y, const ValueType& z)
{
	return MatrixStorage<ValueType, 4, 4>{
		1,			0,			0,			0,
		0,			1,			0,			0,
		0,			0,			1,			0,
		x,			y,			z,			1
	};
}

// create a translation matrix
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateTranslaionMatrixFromVector(const VectorStorage<ValueType,3>& vec)
{
	return CreateTranslaionMatrix(vec.X(), vec.Y(), vec.Z());
}

// create a scaling matrix
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateScalingMatrix(const ValueType& x, const ValueType& y, const ValueType& z)
{
	return MatrixStorage<ValueType, 4, 4>{
		x,			0,			0,			0,
		0,			y,			0,			0,
		0,			0,			z,			0,
		0,			0,			0,			1
	};
}

// create a projection matrix
template<class ValueType>
inline MatrixStorage<ValueType, 4, 4> CreateProjMatrix(const ValueType& fieldOfView, const ValueType& aspectHByW, const ValueType& nearZ, const ValueType& farZ)
{
	// copied from DirectX::XMMatrixPerspectiveFovLH
	ValueType fov = fieldOfView * (ValueType)DEG_TO_RAD / (ValueType)2;
	ValueType sinFov = sin(fov);
	ValueType cosFov = cos(fov);

	ValueType height = cosFov / sinFov;
	ValueType width = height / aspectHByW;
	ValueType range = farZ / (farZ - nearZ);

	return MatrixStorage<ValueType, 4, 4> {
		width,			(ValueType)0,	(ValueType)0,	(ValueType)0,
		(ValueType)0,	height,			(ValueType)0,	(ValueType)0,
		(ValueType)0,	(ValueType)0,	range,			(ValueType)1,
		(ValueType)0,	(ValueType)0,	-range * nearZ,	(ValueType)0
	};
}

template <class ValueType, int RowNum, int ColNum>
bool operator==(const MatrixStorage<ValueType, RowNum, ColNum>& left, const MatrixStorage<ValueType, RowNum, ColNum>& right)
{
	return left.equal(right);
}

template <class ValueType, int RowNum_left, int ColNum_left, int RowNum_right, int ColNum_right>
MatrixStorage<ValueType, RowNum_left, ColNum_right> operator*(const MatrixStorage<ValueType, RowNum_left, ColNum_left>& left,
															  const MatrixStorage<ValueType, RowNum_right, ColNum_right>& right)
{
	return left.dot(right);
}

//////////////////////////////////////////////////////////////////////////
// Member function
//////////////////////////////////////////////////////////////////////////

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>::MatrixStorage()
{
	reset();
}

template <class ValueType, int RowNum, int ColNum>
template <class>
MatrixStorage<ValueType, RowNum, ColNum>::MatrixStorage(const VectorStorage<ValueType, ColNum>& vector)
{
	reset(vector);
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>::MatrixStorage(std::initializer_list<ValueType> valueList)
{
reset(valueList);
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>::MatrixStorage(const ValueType* data)
{
	reset(data);
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>::MatrixStorage(const MatrixStorage& other)
{
	reset(other);
}

template <class ValueType, int RowNum, int ColNum>
VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator()(int index)
{
	return getRow(index);
}

template <class ValueType, int RowNum, int ColNum>
const VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator()(int index) const
{
	return getRow(index);
}

template <class ValueType, int RowNum, int ColNum>
ValueType& MatrixStorage<ValueType, RowNum, ColNum>::operator()(int row, int col)
{
	return _data._row[row](col);
}

template <class ValueType, int RowNum, int ColNum>
const ValueType& MatrixStorage<ValueType, RowNum, ColNum>::operator()(int row, int col) const
{
	return _data._row[row](col);
}

template <class ValueType, int RowNum, int ColNum>
template <class>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator=(const VectorStorage<ValueType, ColNum>& vector)
{
	reset(vector);
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator=(const MatrixStorage<ValueType, RowNum, ColNum>& other)
{
	reset(other);
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator=(std::initializer_list<ValueType> valueList)
{
	reset(valueList);
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::operator=(const ValueType* data)
{
	reset(data);
	return *this;
}

//template <class ValueType, int RowNum, int ColNum>
//MatrixStorage<ValueType, RowNum, ColNum>::operator VectorStorage<ValueType, ColNum>() const
//{
//	return _data._row[0];
//}

template <class ValueType, int RowNum, int ColNum>
void MatrixStorage<ValueType, RowNum, ColNum>::setRow(int row, const VectorStorage<ValueType, ColNum>& val)
{
	_data._row[row] = val;
}

template <class ValueType, int RowNum, int ColNum>
VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::getRow(int row)
{
	return _data._row[row];
}

template <class ValueType, int RowNum, int ColNum>
const VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::getRow(int row) const
{
	return _data._row[row];
}

template <class ValueType, int RowNum, int ColNum>
template <int RowCount, int ColCount>
MatrixStorage<ValueType, RowCount, ColCount> MatrixStorage<ValueType, RowNum, ColNum>::getPartOfMatix(int startRow, int startCol) const
{
	MatrixStorage<ValueType, RowCount, ColCount> target;
	for (int i = 0; i < RowCount; ++i)
	{
		for(int j = 0; j < ColCount; ++j)
		{
			target(i, j) = (*this)(i + startRow, j + startCol);
		}
	}
	return target;
}

template <class ValueType, int RowNum, int ColNum>
const int MatrixStorage<ValueType, RowNum, ColNum>::getRowNum() const
{
	return RowNum;
}

template <class ValueType, int RowNum, int ColNum>
const int MatrixStorage<ValueType, RowNum, ColNum>::getColNum() const
{
	return ColNum;
}

template <class ValueType, int RowNum, int ColNum>
const MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::identity()
{
	static_assert(RowNum == ColNum, "This function can only be called when RowNum == ColNum");
	static MatrixStorage<ValueType, RowNum, ColNum> matrix;
	matrix.identityInPlace();
	return matrix;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::identityInPlace()
{
	static_assert(RowNum == ColNum, "This function can only be called when RowNum == ColNum");
	reset();
	for (int i = 0; i < RowNum; ++i)
	{
		_data._d[i + i * ColNum] = (ValueType)1;
	}
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, ColNum, RowNum> MatrixStorage<ValueType, RowNum, ColNum>::transpose() const
{
	MatrixStorage<ValueType, ColNum, RowNum> target;
	for (int i = 0; i < RowNum; ++i)
	{
		for (int j = 0; j < ColNum; ++j)
		{
			target(j, i) = (*this)(i, j);
		}
	}
	return target;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::transposeInPlace()
{
	static_assert(RowNum == ColNum, "transposeInPlace can only be used on RowNum == ColNum matrix. You can use transpose() instead.");
	for (int i = 0; i < RowNum; ++i)
	{
		for (int j = i + 1; j < ColNum; ++j)
		{
			std::swap((*this)(i, j), (*this)(j, i));
		}
	}
	return *this;
}

//template <class ValueType, int RowNum, int ColNum>
//MatrixStorage<ValueType, RowNum, ColNum> MatrixStorage<ValueType, RowNum, ColNum>::normalized() const
//{
//
//}
//
//template <class ValueType, int RowNum, int ColNum>
//MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::normalizeInPlace()
//{
//}

template <class ValueType, int RowNum, int ColNum>
void MatrixStorage<ValueType, RowNum, ColNum>::reset()
{
	memset(_data._d, 0, sizeof(_data._d));
}

template <class ValueType, int RowNum, int ColNum>
void MatrixStorage<ValueType, RowNum, ColNum>::reset(const float* data)
{
	memcpy(_data._d, data, sizeof(_data._d));
}

template <class ValueType, int RowNum, int ColNum>
void MatrixStorage<ValueType, RowNum, ColNum>::reset(const MatrixStorage& other)
{
	memcpy(_data._d, other._data._d, sizeof(_data._d));
}

template <class ValueType, int RowNum, int ColNum>
void MatrixStorage<ValueType, RowNum, ColNum>::reset(std::initializer_list<ValueType> valueList)
{
	for (int i = 0; i < ColNum * RowNum; ++i)
	{
		if (i < (int)valueList.size())
			_data._d[i] = *(valueList.begin() + i);
		else
			_data._d[i] = 0;
	}
}

template <class ValueType, int RowNum, int ColNum>
template <class>
void MatrixStorage<ValueType, RowNum, ColNum>::reset(const VectorStorage<ValueType, ColNum>& vector)
{
	_data._row[0] = vector;
}

template <class ValueType, int RowNum, int ColNum>
ValueType* MatrixStorage<ValueType, RowNum, ColNum>::getData()
{
	return _data._d;
}

template <class ValueType, int RowNum, int ColNum>
const ValueType* MatrixStorage<ValueType, RowNum, ColNum>::getData() const
{
	return _data._d;
}

template <class ValueType, int RowNum, int ColNum>
template <int RowNum_other, int ColNum_other>
inline MatrixStorage<ValueType, RowNum, ColNum_other> MatrixStorage<ValueType, RowNum, ColNum>::dot(const MatrixStorage<ValueType, RowNum_other, ColNum_other>& other) const
{
	static_assert(ColNum == RowNum_other, "Matrix multiply needs left ColNum == right RowNum");
	MatrixStorage<ValueType, RowNum, ColNum_other> target;
	VectorStorage<ValueType, RowNum_other> col;
	for (int i = 0; i < RowNum; ++i)
	{
		for (int j = 0; j < ColNum_other; ++j)
		{
			col = other.getPartOfMatix<RowNum_other,1>(0,j).getData();
			target(i, j) = _data._row[i] * col;
		}
	}
	return target;
}

template <class ValueType, int RowNum, int ColNum>
inline MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::dotInPlace(const MatrixStorage<ValueType, ColNum, ColNum>& other)
{
	VectorStorage<ValueType, ColNum> row;
	VectorStorage<ValueType, ColNum> col;
	for (int i = 0; i < RowNum; ++i)
	{
		row = _data._row[i];
		for (int j = 0; j < ColNum; ++j)
		{
			col = other.getPartOfMatix<ColNum, 1>(0, j).getData();
			_data._row[i](j) = row * col;
		}
	}
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum> MatrixStorage<ValueType, RowNum, ColNum>::scaled(const ValueType& value) const
{
	MatrixStorage<ValueType, RowNum, ColNum> target = *this;
	for (int i = 0; i < RowNum * ColNum; ++i)
	{
		target._data._d[i] *= value;
	}
	return target;
}

template <class ValueType, int RowNum, int ColNum>
MatrixStorage<ValueType, RowNum, ColNum>& MatrixStorage<ValueType, RowNum, ColNum>::scaleInPlace(const ValueType& value)
{
	for (int i = 0; i < RowNum * ColNum; ++i)
	{
		_data._d[i] *= value;
	}
	return *this;
}

template <class ValueType, int RowNum, int ColNum>
bool MatrixStorage<ValueType, RowNum, ColNum>::equal(const MatrixStorage<ValueType, RowNum, ColNum>& other) const
{
	for (int i = 0; i < ColNum * RowNum; ++i)
	{
		if (!isEqual(_data._d[i], other._data._d[i]))
			return false;
	}
	return true;
}
