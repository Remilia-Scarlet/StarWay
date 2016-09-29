
template <class ValueType, int ColNum, int RowNum>
MatrixStorage<ValueType, ColNum, RowNum>::MatrixStorage()
{

}

template <class ValueType, int ColNum, int RowNum>
MatrixStorage<ValueType, ColNum, RowNum>::MatrixStorage(std::initializer_list<ValueType> valueList)
{

}

template <class ValueType, int ColNum, int RowNum>
MatrixStorage<ValueType, ColNum, RowNum>::MatrixStorage(const float* data)
{

}

template <class ValueType, int ColNum, int RowNum>
VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, ColNum, RowNum>::operator()(int index)
{

}

template <class ValueType, int ColNum, int RowNum>
const VectorStorage<ValueType, ColNum>& MatrixStorage<ValueType, ColNum, RowNum>::operator()(int index) const
{

}

template <class ValueType, int ColNum, int RowNum>
ValueType& MatrixStorage<ValueType, ColNum, RowNum>::operator()(int row, int col)
{

}
template <class ValueType, int ColNum, int RowNum>
const ValueType& MatrixStorage<ValueType, ColNum, RowNum>::operator()(int row, int col) const
{

}

template <class ValueType, int ColNum, int RowNum>
MatrixStorage& MatrixStorage<ValueType, ColNum, RowNum>::operator=(const MatrixStorage& other)
{

