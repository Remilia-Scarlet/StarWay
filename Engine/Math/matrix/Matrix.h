#pragma once
#include "Math\vector\Vector.h"
template <class ValueType, int RowNum, int ColNum>
class MatrixStorage
{
public:
	// Construct a matix with all elements 0.
	inline MatrixStorage();

	// If row count is 1, the matix can be constructed with a Vector.
	template <class = typename std::enable_if<RowNum == 1, void>::type>
	inline MatrixStorage(const VectorStorage<ValueType, ColNum>& vector);

	// Initilize matrix by {....}. For example:
	// Matrix<float,2,3> matrix = {1,0,0, 0,0,1};
	inline MatrixStorage(std::initializer_list<ValueType> valueList);

	// Initilize matrix by data.
	inline MatrixStorage(const ValueType* data);

	// Copy another matix
	inline MatrixStorage(const MatrixStorage& other);

	inline ~MatrixStorage() {}

	// Get a row
	inline VectorStorage<ValueType, ColNum>& operator()(int index);
	inline const VectorStorage<ValueType, ColNum>& operator()(int index) const;

	// Get an element
	inline ValueType& operator()(int row, int col);
	inline const ValueType& operator()(int row, int col) const;

	// If row count is 1, the matix be set by a Vector.
	template <class = typename std::enable_if<RowNum == 1, void>::type>
	inline MatrixStorage& operator=(const VectorStorage<ValueType, ColNum>& vector);

	// copy another matrix
	inline MatrixStorage& operator=(const MatrixStorage& other);

	// Set matrix by{ .... }.For example :
	// Matrix<float,2,3> matrix; matrix = {1,0,0, 0,0,1};
	inline MatrixStorage& operator=(std::initializer_list<ValueType> valueList);

	// Set matix by data
	inline MatrixStorage& operator=(const ValueType* data);

	//// Cast this matix to vector if RowNum is 1
	//inline operator VectorStorage<ValueType, ColNum>() const;

	// Set a row
	inline void setRow(int row, const VectorStorage<ValueType, ColNum>& val);

	// Get a row
	inline VectorStorage<ValueType, ColNum>& getRow(int row);
	inline const VectorStorage<ValueType, ColNum>& getRow(int row) const;

	// Get a part of this matix. Return the new size matrix. For example:
	// MatrixStorage<float,3,4> matrix= {1,2,3,4, 5,6,7,8, 9,10,11,12};
	// MatrixStorage<float,2,2> partMatrix = matrix.getPartOfMatix<2,2>(1,2);//(means start from row1 col2, get a 2*2 matrix)
	// Now partMatrix is |7 ,8 |
	//                   |11,12|
	template <int RowCount, int ColCount>
	inline MatrixStorage<ValueType, RowCount, ColCount> getPartOfMatix(int startRow,int startCol) const;

	// Return row count
	inline const int getRowNum() const;

	// Return col count
	inline const int getColNum() const;

	// Return an identity matrix. Can be only used in Matrix ColNum == RowNum. See also identityInPlace()
	static inline const MatrixStorage& identity();

	// Set this matrix to identity and return its ref. Can be only used in Matrix ColNum == RowNum. See also identity()
	inline MatrixStorage& identityInPlace();

	// Return a copy of transpose matrix. See also transposeInPlace()
	inline MatrixStorage<ValueType, ColNum, RowNum> transpose() const;

	// Transpose this matrix and return its ref. See also transpose()
	inline MatrixStorage& transposeInPlace();

	// Return a copy of normalized matrix. See also normalizeInPlace()
//	inline MatrixStorage normalized() const;

	// Normalize the matrix and return its ref. See also normalized()
//	inline MatrixStorage& normalizeInPlace();

	// reset to 0
	inline void reset();

	// reset by data
	inline void reset(const float* data);

	// reset by another matix
	inline void reset(const MatrixStorage& other);

	// reset by initializer_list
	inline void reset(std::initializer_list<ValueType> valueList);

	// If row count is 1, the matix be set by a Vector.
	template <class = typename std::enable_if<RowNum == 1, void>::type>
	inline void reset(const VectorStorage<ValueType, ColNum>& vector);

	// return the data
	inline ValueType* getData();
	inline const ValueType* getData() const;

	// Dot this matrix and other one, and return the copy. Both this and the other one will not be modified. See also dotInPlace()
	template <int RowNum_other, int ColNum_other>
	inline MatrixStorage<ValueType, RowNum, ColNum_other> dot(const MatrixStorage<ValueType, RowNum_other, ColNum_other>& other) const;

	// Dot this matrix and other one, and set the result to this. Return ref of this. See also dot()
	inline MatrixStorage<ValueType, RowNum, ColNum>& dotInPlace(const MatrixStorage<ValueType, ColNum, ColNum>& other);

	// Multi all elements with value. Return copy. See also scaleInPlace()
	inline MatrixStorage<ValueType, RowNum, ColNum> scaled(const ValueType& value) const;

	// Multi all elements with value. Return this. See also scaled()
	inline MatrixStorage<ValueType, RowNum, ColNum>& scaleInPlace(const ValueType& value);
protected:
	union Data
	{
		Data()
		{
			for (int i = 0; i < RowNum; ++i)
			{
				new (&_row[i]) VectorStorage<ValueType, ColNum>();
			}
		}
		~Data()
		{
			for (int i = 0; i < RowNum; ++i)
			{
				_row[i].~VectorStorage();
			}
		}
		VectorStorage<ValueType, ColNum> _row[RowNum];
		ValueType _d[ColNum * RowNum];
	}_data;
};

#include "Matrix.inl"

typedef MatrixStorage<float, 2, 2> Matrix2;
typedef MatrixStorage<float, 3, 3> Matrix3;
typedef MatrixStorage<float, 4, 4> Matrix4;
typedef MatrixStorage<float, 4, 3> Matrix43;