#include "WinLancher/precomp.h"
#include "Math/matrix/Matrix.h"

void TestMatrix()
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

	MatrixStorage<float, 3, 2> madas = mmmm.scaled(5);
	mmmm.scaleInPlace(5);

	//if (mmmm == mmmm2)
	//{
	//	int a = 0;
	//}
	if (madas == mmmm)
	{
		int a = 0;
	}
	MatrixStorage<double, 2, 2> ddd = { 1.1,2.2,3.3,4.4 };
	MatrixStorage<double, 2, 2> ddd2 = ddd;
	if (ddd == ddd2)
	{
		int a = 0;
	}

}

void TestVector()
{
	Vector4 v4;
	Vector4 v41 = { 1,2 };
	Vector4 v42 = { 1,2,3,4,5,6,7 };
	float dd[] = { 1,2,3,4,5,6,7,8,9,10 };
	Vector4 v43 = dd;
	Vector4 v44 = 1;
	Vector4 v45 = v44;

	float ff = v42(1);
	ff = const_cast<const Vector4&>(v42)(2);

	float* da1 = v42.getData();
	const float* da2 = const_cast<const Vector4&>(v42).getData();

	v42 = { 1.1f,1.2f,1.3f,1.4f,1.5f };
	float& x = v42.X();
	float& y = v42.Y();
	float& z = v42.Z();
	float& w = v42.W();
	const float& cx = const_cast<const Vector4&>(v42).X();
	const float& cy = const_cast<const Vector4&>(v42).Y();
	const float& cz = const_cast<const Vector4&>(v42).Z();
	const float& cw = const_cast<const Vector4&>(v42).W();

	v42 = dd;
	v42 = 999;
	v42 = v44;

	v42.reset();
	v42.reset({ 1.1f });
	v42.reset(ff);
	v42.reset(dd);
	v42.reset(v44);

	v42.setValue(1, 999);
	float aa = v42.dot({ 1,2,3,4 });
	auto dsaf = v42.scaled(2);
	v42.scaleInPlace(2);

	auto fdsafsa= v42.normalized();
	v42.normalizeInPlace();;

	if (v41 == v42)
	{
		int a = 0;
	}
	v41 = v42;
	if (v41 == v42)
	{
		int a = 0;
	}

	VectorStorage<int, 3> vii = { 1,2,3 };
	//if (v41 == vii)
	//{
	//	int a = 0;
	//}
	VectorStorage<int, 3> vii2 = { 1,2,3 };
	if (vii == vii2)
	{
		int a = 0;
	}
	float dot = v43 * v42;
	Vector4 vvv = 2.f * v43;

	float l = v42.lenth();

	int a = 0;
}

void UnitTest()
{
	TestMatrix();
	TestVector();
}