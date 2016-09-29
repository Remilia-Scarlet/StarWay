#pragma once
#include "Math\vector\Vector.h"
template <class ValueType, int ColNum, int RowNum>
class MatrixStorage
{
public:
	inline MatrixStorage();
	inline MatrixStorage(std::initializer_list<ValueType> valueList);
	inline MatrixStorage(const float* data);
	inline ~MatrixStorage() {}

	inline VectorStorage<ValueType, ColNum>& operator()(int index);
	inline const VectorStorage<ValueType, ColNum>& operator()(int index) const;
	inline ValueType& operator()(int row, int col);
	inline const ValueType& operator()(int row, int col) const;

	inline MatrixStorage& operator=(const MatrixStorage& other);

	inline void setMatrix(const float* data);
	inline void setRow(const VectorStorage<ValueType, ColNum>& row);
	inline VectorStorage<ValueType, ColNum>& getRow();
	inline const VectorStorage<ValueType, ColNum>& getRow() const;
	inline constexpr int getRowNum();
	inline constexpr int getColNum();
	inline MatrixStorage identity()const;
	inline MatrixStorage& identityInPlace();
	inline MatrixStorage transpose() const;
	inline MatrixStorage& transposeInPlace();
	inline void reset();
	inline ValueType* getData();
	inline const ValueType* getData() const;
protected:
	union {
		VectorStorage<ValueType, ColNum> _m[RowNum];
		ValueType _data[ColNum * RowNum];
	};
	
};

template <int ColNum, int RowNum>
bool operator==(const MatrixStorage<float, ColNum, RowNum>& left, const MatrixStorage<float, ColNum, RowNum>& right)
{
	for (int i = 0; i < ColNum * RowNum; ++i)
	{
		if (!TINY_FLOAT_EQUAL(*(left.getData() + i), *(right.getData() + i)))
			return false;
	}
	return true;
}

template <class ValueType, int ColNum, int RowNum>
bool operator==(const MatrixStorage<ValueType, ColNum, RowNum>& left, const MatrixStorage<ValueType, ColNum, RowNum>& right)
{
	return memcmp(left.getData(), right.getData(), sizeof(MatrixStorage<ValueType, ColNum, RowNum>));
}

#include "Matrix.inl"

