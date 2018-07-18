#include "UnitTest/precomp.h"
#include "gtest/gtest.h"
#include "Math/MathDef.h"
#include "Math/matrix/Matrix.h"
//#include <DirectXMath.h>

TEST(Math, FloatEpsionTest)
{
	float f1 = 1.0f;
	float f2 = f1 + FLT_EPSILON;
	EXPECT_LT(f1, f2);
	EXPECT_FALSE(isGreater(f2, f1));
	
	float a = 1.2f;
	float b = 1.3f;
	EXPECT_TRUE(TINY_FLOAT_EQUAL(a, 1.2f));
	EXPECT_TRUE(TINY_FLOAT_EQUAL(a, 1.20001f));
	EXPECT_TRUE(TINY_FLOAT_EQUAL(b, 1.3f));
	EXPECT_FALSE(TINY_FLOAT_EQUAL(b, 1.30001f)); //Shit!

	float c = a + 0.1f;
	EXPECT_TRUE(TINY_FLOAT_EQUAL(c, b));

	auto minus = abs(c - b);
	EXPECT_EQ(minus, FLT_EPSILON);

	c += 0.000001f;
	EXPECT_TRUE(TINY_FLOAT_EQUAL(c, b));
}

TEST(Math, MatixTest)
{
	{
		Matrix4 ma;
		EXPECT_EQ(ma(0), Vector4());
		EXPECT_EQ(ma(1), Vector4());
		EXPECT_EQ(ma(2), Vector4());
		EXPECT_EQ(ma(3), Vector4());
	}
	{
		Vector4 v4 = { 1,2,3,4 };
		MatrixStorage<float, 1, 4> mv(v4);
		EXPECT_EQ(mv(0), v4);
	}

	{
		MatrixStorage<float, 2, 4> ma2 = { 1,2,3,4, 5,6,7,8 };
		EXPECT_EQ(ma2.getRow(0), Vector4(1, 2, 3, 4));
		EXPECT_EQ(ma2.getRow(1), Vector4(5, 6, 7, 8));
	}

	{
		float dd[] = { 11,12,13,14, 15,16,17,18 };
		MatrixStorage<float, 2, 4> ma3 = dd;
		MatrixStorage<float, 2, 4> ma4 = ma3;
		EXPECT_EQ(ma4.getRow(0), Vector4(11, 12, 13, 14));
		EXPECT_EQ(ma4.getRow(1), Vector4(15, 16, 17, 18));

		Vector4& v = ma3(1);
		v = { 1.1f,1.2f,1.3f,1.4f };
		EXPECT_EQ(ma3.getRow(1), Vector4(1.1f, 1.2f, 1.3f, 1.4f));

		float& f = ma4(1, 3);
		f = 999;
		EXPECT_EQ(ma4.getRow(1), Vector4(15, 16, 17, 999));
	}

	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		ma = { 999,998,997,996,995,994 };
		EXPECT_EQ(ma.getRow(0), Vector4(999, 998, 997, 996));
		EXPECT_EQ(ma.getRow(1), Vector4(995, 994, 0, 0));

		ma = { 1,2,3,4,5,6,7,8,9,0, 999,998,997,996,995,994 };
		EXPECT_EQ(ma.getRow(0), Vector4(1, 2, 3, 4));
		EXPECT_EQ(ma.getRow(1), Vector4(5, 6, 7, 8));
	}

	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		ma.setRow(1, { 100,101,102,103 });
		EXPECT_EQ(ma.getRow(0), Vector4(1, 2, 3, 4));
		EXPECT_EQ(ma.getRow(1), Vector4(100, 101, 102, 103));
	}
	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		Vector4& vvv = ma.getRow(1);
		vvv(2) = 250;
		EXPECT_EQ(ma.getRow(0), Vector4(1, 2, 3, 4));
		EXPECT_EQ(ma.getRow(1), Vector4(5, 6, 250, 8));
	}

	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		int aa = ma.getRowNum();
		int bb = ma.getColNum();
		EXPECT_EQ(aa, 2);
		EXPECT_EQ(bb, 4);
	}
	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		float* data = ma.getData();
		for (int i = 0; i < 8; ++i)
		{
			EXPECT_FLOAT_EQ(data[i], (float)i + 1.f);
		}
	}
	{
		MatrixStorage<float, 3, 4> matrix = { 1,2,3,4, 5,6,7,8, 9,10,11,12 };
		MatrixStorage<float, 2, 2> partMatrix = matrix.getPartOfMatix<2, 2>(1, 2);
		EXPECT_EQ(partMatrix.getRow(0), Vector2(7, 8));
		EXPECT_EQ(partMatrix.getRow(1), Vector2(11, 12));
	}
	{
		auto idt = MatrixStorage<float, 4, 4> ::identity();
		EXPECT_EQ(idt.getRow(0), Vector4(1, 0, 0, 0));
		EXPECT_EQ(idt.getRow(1), Vector4(0, 1, 0, 0));
		EXPECT_EQ(idt.getRow(2), Vector4(0, 0, 1, 0));
		EXPECT_EQ(idt.getRow(3), Vector4(0, 0, 0, 1));

		Matrix4 ma;
		ma.identityInPlace();
		EXPECT_EQ(ma, idt);
	}
	{
		MatrixStorage<float, 2, 4> ma = { 1,2,3,4, 5,6,7,8 };
		MatrixStorage<float, 4, 2> mm = ma.transpose();
		EXPECT_EQ(mm.getRow(0), Vector2(1, 5));
		EXPECT_EQ(mm.getRow(1), Vector2(2, 6));
		EXPECT_EQ(mm.getRow(2), Vector2(3, 7));
		EXPECT_EQ(mm.getRow(3), Vector2(4, 8));
	}
	{
		MatrixStorage<float, 3, 2> m1 = { 1,2, 3,4, 5,6 };
		MatrixStorage<float, 2, 3> m2 = { 11,12,13, 14,15,16 };
		MatrixStorage<float, 3, 3> m3 = m1 * m2;
		Matrix3 answer = { 39,42,45, 89,96,103, 139,150,161 };
		EXPECT_EQ(m3, answer);

	}
	{
		MatrixStorage<float, 3, 2> m1 = { 1,2, 3,4, 5,6 };
		MatrixStorage<float, 2, 2> m2 = { 7,8, 9,10 };
		m1.dotInPlace(m2);
		MatrixStorage<float, 3, 2> answer = { 25,28, 57,64, 89,100};
		EXPECT_EQ(m1, answer);
	}
	{
		auto idt = MatrixStorage<float, 2, 2> ::identity();
		auto idt5 = idt.scaled(5);
		idt.scaleInPlace(5);
		EXPECT_EQ(idt(0)(0), 5);
		EXPECT_EQ(idt(0)(1), 0);
		EXPECT_EQ(idt(1)(0), 0);
		EXPECT_EQ(idt(1)(1), 5);
		EXPECT_EQ(idt, idt5);
	}
}

TEST(Math, VectorTest)
{
	{
		Vector4 v4;
		EXPECT_EQ(v4(0), 0);
		EXPECT_EQ(v4(1), 0);
		EXPECT_EQ(v4(2), 0);
		EXPECT_EQ(v4(3), 0);
	}
	{
		Vector4 v4 = { 1,2 };
		EXPECT_EQ(v4.X(), 1);
		EXPECT_EQ(v4.Y(), 2);
		EXPECT_EQ(v4.Z(), 0);
		EXPECT_EQ(v4.W(), 0);
		Vector4 v42 = { 1,2,3,4,5,6,7 };
		EXPECT_EQ(v42.X(), 1);
		EXPECT_EQ(v42.Y(), 2);
		EXPECT_EQ(v42.Z(), 3);
		EXPECT_EQ(v42.W(), 4);
		auto minu = v42 - v4;
		EXPECT_EQ(minu.X(), 0);
		EXPECT_EQ(minu.Y(), 0);
		EXPECT_EQ(minu.Z(), 3);
		EXPECT_EQ(minu.W(), 4);
	}
	{
		float dd[] = { 1,2,3,4,5,6,7,8,9,10 };
		Vector4 v43 = dd;
		EXPECT_EQ(v43, (Vector4{ 1,2,3,4 }));
		Vector2 sub = v43.subVecter<1, 2>();
		Vector4 sub2 = v43.subVecter<3, 4>();
		EXPECT_EQ(sub, (Vector2{ 2,3 }));
		EXPECT_EQ(sub2, (Vector4{ 4,0,0,0 }));
	}
	{
		Vector3 v42 = { 1,2,3 };
		float ff = v42(1);
		EXPECT_EQ(ff, 2);
	}
	{
		Vector3 v42 = { 1,2,3 };
		float* da1 = v42.getData();
		EXPECT_EQ(da1[0], 1);
		EXPECT_EQ(da1[1], 2);
		EXPECT_EQ(da1[2], 3);

		float& x = v42.X();
		float& y = v42.Y();
		float& z = v42.Z();

		x = 233;
		y = 244;
		z = 255;
		EXPECT_EQ(v42, (Vector3{ 233,244,255 }));

		v42.setValue(1, 999);
		EXPECT_EQ(v42, (Vector3{ 233,999,255 }));
	}
	{
		Vector3 v42 = { 1,2,3 };
		v42 = 999;
		EXPECT_EQ(v42, (Vector3{ 999,999,999 }));
	}

	{
		Vector3 v42 = { 1,2,3 };
		float aa = v42.dot({ 4,5,6 });
		EXPECT_EQ(aa, 32);

		auto v42s = v42.scaled(2);
		v42.scaleInPlace(2);
		EXPECT_EQ(v42, v42s);
		EXPECT_EQ(v42, (Vector3{ 2,4,6 }));
	}
	{
		Vector3 v42 = { 1,2,3 };
		auto norm = v42.normalized();
		v42.normalizeInPlace();
		EXPECT_EQ(v42, norm);
		EXPECT_EQ(v42, (Vector3{ 1.f / 3.74165738f,2.f / 3.74165738f,3.f / 3.74165738f }));
	}
	{
		//For other data type
		VectorStorage<int, 3> vii = { 1,2,3 };
		VectorStorage<int, 3> vii2 = { 1,2,3 };
		VectorStorage<double, 3> ddd1 = { 1.1,1.2,1.3 };
		VectorStorage<double, 3> ddd2 = { 1.1,1.2,1.3 };
	}

	{
		Vector3 v42 = { 1,2,3 };
		Vector3 v43 = { 4,5,6 };
		float dot = v43 * v42;
		EXPECT_EQ(dot, 32);
		Vector3 vvv = 2.f * v42;
		EXPECT_EQ(vvv(0), 2);
		EXPECT_EQ(vvv(1), 4);
		EXPECT_EQ(vvv(2), 6);
	}
	{
		Vector3 v42 = { 1,2,3 };
		float l = v42.lenth();
		float l2 = v42.lenth2();
		EXPECT_EQ(l, 3.74165738f);
		EXPECT_EQ(l2, 14);
	}
	{
		Vector3 cccc1 = { 1,2,3 };
		Vector3 cccc2 = { 4,5,6 };
		Vector3 ccccc = cccc1.cross(cccc2);
		cccc1.crossInPlace(cccc2);
		EXPECT_EQ(ccccc, cccc1);
		EXPECT_EQ(ccccc, (Vector3{ -3,6,-3 }));
	}
	{
		Vector3 cccc1 = { 1,2,3 };
		Vector3 cccc2 = { 4,5,6 };
		auto ccccc = cccc1.multiply(cccc2);
		cccc1.multiplyInPlace(cccc2);
		EXPECT_EQ(ccccc, cccc1);
		EXPECT_EQ(ccccc, (Vector3{ 4,10,18 }));
	}
	{
		Vector3 v111 = { 1,1,1 };
		Quaternion rotate(Vector3{ 1,0,0 }, 90.f);
		Quaternion rotate2(Vector3{ 0,1,0 }, 90.f);
		Quaternion rotate3(Vector3{ 0,0,1 }, 90.f);
		auto v111r1 = v111.rotate(rotate);
		EXPECT_EQ(v111r1, (Vector3{ 1,-1,1 }));
		auto v111r2 = v111.rotate(rotate2);
		EXPECT_EQ(v111r2, (Vector3{ 1,1,-1 }));
		auto v111r3 = v111.rotate(rotate3);
		EXPECT_EQ(v111r3, (Vector3{ -1,1,1 }));
		v111.rotateInPlace(rotate3);
		EXPECT_EQ(v111r3, v111);
		v111.rotateInPlace(Quaternion{ Vector3{ 1, 1, 1 }, 45.f });
		EXPECT_EQ(v111, (Vector3{ -0.609475613f,-0.0117586851f,1.62123442f }));
	}
}

TEST(Math, QuaternionTest)
{
	//DirectX::XMMATRIX dxma1 = DirectX::XMMatrixRotationZ(degToRad(30.0f));
	//DirectX::XMVECTOR qua1 = DirectX::XMQuaternionRotationMatrix(dxma1); //qua1 == {0,0,0.258819044,0.965925872}
	Quaternion rotate1(Vector3{ 0,0,30 });
	Quaternion copyQua = rotate1;
	EXPECT_FLOAT_EQ(copyQua.X(), 0);
	EXPECT_FLOAT_EQ(copyQua.Y(), 0);
	EXPECT_FLOAT_EQ(copyQua.Z(), 0.258819044f);
	EXPECT_FLOAT_EQ(copyQua.W(), 0.965925872f);

	//DirectX::XMVECTOR v = { 0,1,0,0 };
	//DirectX::XMVECTOR qua2 = DirectX::XMQuaternionRotationAxis(v, degToRad(45.0f)); //qua2 == {0, 0.382683426, 0, 0.923879504}
	//DirectX::XMMATRIX dxma2 = DirectX::XMMatrixRotationQuaternion(qua2);
	Quaternion rotate2(Vector3{ 0,1,0 }, 45);
	EXPECT_EQ(rotate2, (Quaternion{ 0.923879504f, 0, 0.382683426f, 0 }));

	//DirectX::XMVECTOR qua3 = DirectX::XMQuaternionRotationRollPitchYaw(degToRad(60.f), 0, 0);//qua3 == {0.5, 0, 0, 0.866025388}
	//DirectX::XMMATRIX dxma3 = DirectX::XMMatrixRotationQuaternion(qua2);
	Quaternion rotate3(60.f, 0, 0);
	EXPECT_TRUE(rotate3.equal(Quaternion{ 0.866025388f,0.5, 0, 0 }));

	//DirectX::XMVECTOR qua4 = DirectX::XMQuaternionRotationRollPitchYaw(degToRad(45.f), degToRad(60.f), degToRad(90.f));
	Quaternion rotate4(45.f, 60.f, 90.f);

	//DirectX::XMVECTOR vvv = { 1,1,1,0 };
	Vector3 vecmy = { 1,1,1 };

	//vvv = DirectX::XMVector3Rotate(vvv, qua1); //vvv == {0.366025478, 1.36602557, 1, 0}
	vecmy = vecmy.rotate(rotate1);
	EXPECT_EQ(vecmy, (Vector3{ 0.366025478f, 1.36602557f, 1.f }));

	//vvv = DirectX::XMVector3Rotate(vvv, qua2);// vvv == {0.965925932, 1.36602545, 0.448287755}
	vecmy.rotateInPlace(rotate2);
	EXPECT_EQ(vecmy, (Vector3{ 0.965925932f, 1.36602545f, 0.448287755f }));

	//vvv = DirectX::XMVector3Rotate(vvv, qua3); // vvv == {0.965925932,0.294784129, 1.40715659 }
	vecmy.rotateInPlace(rotate3);
	EXPECT_EQ(vecmy, (Vector3{ 0.965925932f,0.294784129f, 1.40715659f }));

	//vvv = DirectX::XMVector3Rotate(vvv, qua4); //1.30581796, -0.311997145, 1.09430182
	vecmy.rotateInPlace(rotate4);
	EXPECT_EQ(vecmy, (Vector3{ 1.30581796f, -0.311997145f, 1.09430182f }));

	Vector3 eu = rotate4.toEularAngle();
	EXPECT_EQ(eu, (Vector3{ 45.f, 60.f, 90.f }));

	Quaternion from_to(vecmy, Vector3{ 1,1,1 });
	vecmy.rotateInPlace(from_to);
	EXPECT_EQ(vecmy, (Vector3{ 1.f,1.f,1.f }));

	float theta = degToRad(30.f);
	Matrix4 ma = {
		cos(theta),0,-sin(theta),0,
		0,1,0,0,
		sin(theta),0,cos(theta),0,
		0,0,0,1
	};
	Quaternion from_matrix(ma);
	vecmy = vecmy.rotate(from_matrix);
	EXPECT_EQ(vecmy, (Vector3{ 1.36602533f, 1.f,0.366025388f }));

	Matrix4 mmma = from_matrix.toRotationMatrix();
	EXPECT_EQ(mmma, ma);
	
	auto axisAngle = from_matrix.toAxisAngle();
	EXPECT_EQ(axisAngle.first, (Vector3{ 0.f,1.f,0.f }));
	//EXPECT_TRUE(isEqual(axisAngle.second, 30.f)); //shit!

	auto coon = from_matrix.conjugate();
	vecmy.rotateInPlace(coon);
	EXPECT_EQ(vecmy, (Vector3{ 1.f,1.f,1.f }));
}
