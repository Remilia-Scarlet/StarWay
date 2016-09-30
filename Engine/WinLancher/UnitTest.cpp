#include "WinLancher/precomp.h"
#include "Math/matrix/Matrix.h"

void UnitTest()
{
	Matrix4 ma;
	Vector4 v4 = { 1,2,3,4 };
	MatrixStorage<float, 1, 4> mv(v4);
	//	MatrixStorage<float, 2, 4> mv2(v4);
	MatrixStorage<float, 2, 4> ma2 = { 1,2,3,4, 5,6,7,8 };
	float dd[] = { 11,12,13,14, 15,16,17,18 };
	MatrixStorage<float, 2, 4> ma3 = dd;
	{
		MatrixStorage<float, 2, 4> ma4 = ma3;
	}
	Vector4& v = ma3(1);
	v = { 1.1f,1.2f,1.3f,1.4f };

	float& f = ma3(1, 3);
	f = 999;

	mv = v;
	//	ma3 = v;
	ma3 = ma2;
	ma3 = { 999,998,997,996,995,994 };
	ma3 = { 1,2,3,4,5,6,7,8,9,0, 999,998,997,996,995,994 };
	ma3 = dd;

	ma3.setRow(1, { 100,101,102,103 });
	Vector4& vvv = ma3.getRow(1);
	vvv(2) = 250;

	int aa = ma3.getRowNum();
	int bb = ma3.getColNum();

	auto ff = ma3.getData();

	MatrixStorage<float, 3, 4> matrix = { 1,2,3,4, 5,6,7,8, 9,10,11,12 };
	MatrixStorage<float, 2, 2> partMatrix = matrix.getPartOfMatix<2, 2>(1, 2);

	//	auto adfsdffg = MatrixStorage<float, 3, 4> ::identity();
	auto dsfhsdfasf = MatrixStorage<float, 4, 4> ::identity();
	ma.identityInPlace();
	ma(0, 2) = 998;
	//ma3.identityInPlace();

	MatrixStorage<float, 4, 2> mm = ma3.transpose();
	//	ma3.transposeInPlace();
	ma.transposeInPlace();

	MatrixStorage<float, 3, 2> mmmm = { 1,2, 3,4, 5,6 };
	MatrixStorage<float, 2, 3> mmmm2 = { 11,12,13, 14,15,16 };
	MatrixStorage<float, 3, 3> mmmm3 = mmmm*mmmm2;
	MatrixStorage<float, 2, 2> mmmm4 = { 7,8, 9,10 };
	mmmm.dotInPlace(mmmm4);



}