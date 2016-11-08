#include "WinLancher/precomp.h"
#include "Math/matrix/Matrix.h"
#include "Math/quaternion/Quaternion.h"
#include <DirectXMath.h>
#include <functional>
#include "TinyEngine/Other/Timer.h"
#include "Math/shape/Rect.h"
#include "TinyEngine/ScriptManager/LuaVal.h"
#include "TinyEngine/Objects/Object.h"
#include "Ash/FileSystem/Path_Win.h"
#include "Ash/CommonFunc.h"
#include "../Ash/FileSystem/File_Win.h"
#include "../TinyEngine/ScriptManager/LuaManager.h"
#include "ThirdParty/rapidjson/document.h"

void Common()
{
	float o = 1.0f;
	float o2 = o + FLT_EPSILON;
	if (o == o2)
	{
		int a = 0;
	}
	if (isgreater(o2, o))
	{
		int a = 0;
	}
	float a = 1.2f;
	float b = 1.3f;
	if (TINY_FLOAT_EQUAL(a, 1.2f))
	{
		int a = 0;
	}
	if (TINY_FLOAT_EQUAL(a, 1.20001f))
	{
		int a = 0;
	}
	if (TINY_FLOAT_EQUAL(b, 1.3f))
	{
		int a = 0;
	}
	if (TINY_FLOAT_EQUAL(b, 1.30001f))
	{
		int a = 0;
	}
	float c = a + 0.1f;
	auto asdad = abs(c - b);
	if (asdad == FLT_EPSILON)
	{
		int a = 0;
	}
	if (TINY_FLOAT_EQUAL(c,b))
	{
		int a = 0;
	}
	if (isgreater(c, b))
	{
		int a = 0;
	}
	if (isgreater(b, c))
	{
		int a = 0;
	}
	if (isGreater(c, b))
	{
		int a = 0;
	}
	if (isLess(c, b))
	{
		int a = 0;
	}
	if (c == b + FLT_EPSILON)
	{
		int a = 0;
	}
	c += 0.000001f;
	if (TINY_FLOAT_EQUAL(c, b))
	{
		int a = 0;
	}

	float d30 = degToRad(30.0f);
	float r30 = radToDeg(d30);

	std::string str = "1231231231233";
	auto spl = split(str, "123");
//	auto sp2 = split(str, "");
	auto sp22 = split("", "123");
	auto sp3 = split(str, "fdsa");
	std::string str2 = str;
	replaceSubstr(str2, "123", "abcdef");
	std::string str3 = str;
	replaceSubstr(str3, "123", "ab");
	std::string str4 = "";
	replaceSubstr(str4, "123", "ab");
	std::string str5 = str;
	replaceSubstr(str5, "", "ab");
	std::string str6 = str;
	replaceSubstr(str6, "123", "");

	int adsafds = 0;
}

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
	MatrixStorage<int, 2, 2> iii = { 1,2,3,4 };
	MatrixStorage<int, 2, 2> iii2 = iii;
	if (iii == iii2)
	{
		int a = 0;
	}

	auto fdsaf = CreateRotationMatrixX(30.f);
	auto fasdcc = CreateRotationMatrixY(30.f);
	auto fdsafds = CreateRotationMatrixZ(30.f);
	auto vcxa = CreateTranslaionMatrix(123.f, 456.f, 789.f);
	auto vzsd = CreateScalingMatrix(2.f, 3.f, 4.f);
}

void TestVector()
{
	Vector4 v4;
	Vector4 v41 = { 1,2 };
	Vector4 v42 = { 1,2,3,4,5,6,7 };
	auto minusss = v42 - v41;
	float dd[] = { 1,2,3,4,5,6,7,8,9,10 };
	Vector4 v43 = dd;
	Vector2 sub = v43.subVecter<1, 2>();
	Vector4 sub2 = v43.subVecter<3, 4>();

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
	VectorStorage<double, 3> ddd1 = { 1.1,1.2,1.3 };
	VectorStorage<double, 3> ddd2 = { 1.1,1.2,1.3 };
	if(ddd1 == ddd2)
	{
		int a = 0;
	}



	float dot = v43 * v42;
	Vector4 vvv = 2.f * v43;

	float l = v42.lenth();
	float l2 = v42.lenth2();

//	Vector4 crossss = v42.cross(v43);
//	v42.crossInPlace(v43);

	Vector3 cccc1 = { 1,2,3 };
	Vector3 cccc2 = { 4,5,6 };
	Vector3 ccccc = cccc1.cross(cccc2);
	cccc1.crossInPlace(cccc2);
	auto fdsafdas = cccc1.multiply(cccc2);
	cccc1.multiplyInPlace(cccc2);

	Vector3 v111 = { 1,1,1 };
	Quaternion rotate(Vector3{ 1,0,0 }, 90.f);
	Quaternion rotate2(Vector3{ 0,1,0 }, 90.f);
	Quaternion rotate3(Vector3{ 0,0,1 }, 90.f);
	auto v111r1 = v111.rotate(rotate);
	auto v111r2 = v111.rotate(rotate2);
	auto v111r3 = v111.rotate(rotate3);
	v111.rotateInPlace(rotate3);
	v111.rotateInPlace(Quaternion{Vector3{1, 1, 1}, 45.f });

	int a = 0;
}
void TestQuaternion()
{
	float theta = degToRad(90.0f);
	DirectX::XMMATRIX dxma = DirectX::XMMatrixRotationY(theta);
	DirectX::XMVECTOR v = { 0,1,0,0 };
	DirectX::XMVECTOR qua = DirectX::XMQuaternionRotationAxis(v, theta);
	DirectX::XMVECTOR qua2 = DirectX::XMQuaternionRotationMatrix(dxma);
	DirectX::XMMATRIX dxma2 = DirectX::XMMatrixRotationQuaternion(qua);
	DirectX::XMVECTOR vvv = { 1,1,1,0 };
	DirectX::XMVECTOR fadsafdsfdas = DirectX::XMVector3Rotate(vvv, qua);

	DirectX::XMVECTOR vv = { 1,1,1,0 };
	DirectX::XMVECTOR quaaa = DirectX::XMQuaternionRotationRollPitchYaw(degToRad(45.f), 0, degToRad(90.f));
	DirectX::XMVECTOR fadsfdas = DirectX::XMVector3Rotate(vv, quaaa);

	Quaternion qqqq;

	Vector3 v111 = { 1,1,1 };
	Quaternion rotate(Vector3{ 0,0,90 });
	Quaternion rotate2(Vector3{0,0,1},90);
	Quaternion rotate3(90, 0, 0);
	Quaternion rotate4(Vector3{ 1,0,0 }, 90);
	Quaternion rotate5(0, 90, 0);
	Quaternion rotate6(Vector3{ 0,1,0 }, 90);
	auto r111 = v111.rotate(rotate);
	auto r222 = v111.rotate(rotate3);
	auto r333 = v111.rotate(rotate5);
	auto eu = rotate2.toEularAngle();
	float deta =90 - eu.Z();
	if (isEqual(eu.Z(), 90.0f))
	{
		int a = 0;
	}
	auto eu2 = rotate4.toEularAngle();
	if (isEqual(eu2.X(), 90.0f))
	{
		int a = 0;
	}
	auto eu3 = rotate6.toEularAngle();
	if (isEqual(eu3.Y(), 90.0f))
	{
		int a = 0;
	}


	Quaternion from_to(v111, r111);
	auto sdfsa = v111.rotate(from_to);

	Quaternion from_to2(Vector3{ 1,0,0 }, Vector3{ 1,0,0 });
	Quaternion from_to3(Vector3{ 1,0,0 }, Vector3{ -1,0,0 });
	sdfsa = v111.rotate(from_to3);



	Matrix4 ma = {
		cos(theta),0,-sin(theta),0,
		0,1,0,0,
		sin(theta),0,cos(theta),0,
		0,0,0,1
	};
	Quaternion from_matrix(ma);
	sdfsa = v111.rotate(from_matrix);
	auto mmma = from_matrix.toRotationMatrix();
	if (mmma == ma)
	{
		int a = 0;
	}
	ma = {
		1,0,0,0,
		0,cos(theta),sin(theta),0,
		0,-sin(theta),cos(theta),0,
		0,0,0,1
	};
	from_matrix = ma;
	sdfsa = v111.rotate(from_matrix);
	mmma = from_matrix.toRotationMatrix();
	if (mmma == ma)
	{
		int a = 0;
	}
	ma = {
		cos(theta),sin(theta),0,0,
		-sin(theta),cos(theta),0,0,
		0,0,1,0,
		0,0,0,1
	};
	from_matrix = ma;
	sdfsa = v111.rotate(from_matrix);
	mmma = from_matrix.toRotationMatrix();
	if (mmma == ma)
	{
		int a = 0;
	}

	auto fdsaf= from_matrix.toAxisAngle();

	auto coon = from_matrix.conjugate();
	coon.conjugateInPlace();

	Quaternion vq = { 0,v111.X(),v111.Y(),v111.Z() };
	auto fsadfgg = from_matrix * vq * from_matrix.conjugate();
	auto fdsag = from_matrix;
	fdsag.productInPlace(vq);
	fdsag.productInPlace(from_matrix.conjugate());

	Quaternion roooo(45,0,90);
	v111 = { 1,1,1 };
	auto erqre = v111.rotate(roooo);



	int a = 0;

}
void TestTimer()
{
	static bool ok = false;
	if (ok)
		return;
	ok = true;

	std::function<void(Timer*)> fibonacii = [](Timer* timer) {
		int* array = (int*)timer->getUserData();
		int value = array[0] + array[1];
		array[0] = array[1];
		array[1] = value;
		DebugString("%d", value);
	};

	Timer timer = Timer(1, 20, fibonacii);

	timer.onStart() = [](Timer* timer) {
		if (timer->getUserData() != nullptr)
			delete timer->getUserData();
		int* array = new int[2];
		array[0] = array[1] = 1;
		timer->setUserData(array);
	};

	timer.onStop() = [](Timer* timer) {
		if (timer->getUserData() != nullptr)
		{
			delete timer->getUserData();
			timer->setUserData(nullptr);
		}
	};

	timer.start();
}
void TestRect()
{
	Rect re;
	Rect re2(1.f, 2.f, 3.f, 4.f);

	Vector2 lo = re2.getLocation();
	Vector2 si = re2.getSize();

	float w = re2.W();
	auto ww = const_cast<const Rect&>(re2).W();

	float h = re2.H();
	auto hh = const_cast<const Rect&>(re2).H();

	float x = re2.X();
	auto xx = const_cast<const Rect&>(re2).X();

	float y = re2.Y();
	auto yy = const_cast<const Rect&>(re2).Y();

	auto le = re2.getLeft();
	auto ri = re2.getRight();
	auto to = re2.getTop();
	auto bo = re2.getBottom();

	bool co = re2.contains(1.f, 1.f);
	bool co2 = re2.contains({ 2,2 });

	if (re == re2)
	{
		int a = 0;
	}
}
void TestLuaVal()
{
	LuaVal val;
	LuaVal vall(nullptr);
	std::string str0 = vall.toString();
	LuaVal valll(false);
	std::string str1 = valll.toString();
	LuaVal val0('a');
	LuaVal val1((signed char)1);
	LuaVal val2(unsigned char(1));
	LuaVal val3((unsigned short)2);
	LuaVal val4((wchar_t)2);
	LuaVal val5((long)1);
	LuaVal val6(unsigned int(44));
	LuaVal val7(long long(33));
	LuaVal val8((unsigned long long) 443);
	std::string str2 = val8.toString();
	LuaVal val9(1.3f);
	LuaVal val10(1.4);
	std::string str3 = val10.toString();
	LuaVal val11("abc");
	LuaVal val12("");
	LuaVal val13(std::string("456"));
	std::string str4 = val13.toString();
	ObjectPtr obj = Object::create();
	LuaVal val14(obj);
	LuaVal val15(obj.get());
	LuaVal val16(*obj.get());
	std::string str5 = val16.toString();
	LuaVal val17 = {1, _K(1.1) = "mmm", "abc", _K("1") = 234, _K(2) = 1, _K("x") = 123, _K("y") = 456, _K("tab") = {1, 2, 3}};
	for (auto& pair : val17)
	{
		const LuaVal& key = pair.first;
		LuaVal& val = pair.second;
		//	DebugString("%s,%s", key.toString().c_str(), val.toString().c_str());
	}
	const LuaVal valconst = val17;
	for (auto it = valconst.begin(); it != valconst.end(); ++it)
	{
		//it->second = 1;
	}


	std::string str6 = val17.toString();
	LuaVal val18 = val17;
	val18.setField(2,999);
	val18.setField(6, "def");
	LuaVal val19 = val18.clone();
	val19.setField("z", { 4,5,6 });
	val14 = val19.getField("z");
	LuaVal val20 = std::move(val19);
	std::string str7 = val20.toString();

	if (val20 != val18)
	{
		int a = 0;
	}
	if (val17 == val18)
	{
		int a = 0;
	}
	LuaVal val21 = 1;
	LuaVal val22 = 1.00000001;
	if (val21 == val22)
	{
		int a = 0;
	}

	LuaVal val23 = val21 + val22;
	LuaVal val24 = val21 - val22;

//	std::list<LuaVal> ret = LuaManager::instance()->call("add", 1, 2.0, "abc", 1.1f, 1.2);

	LuaVal aaasd =  { 123, {1,1,1 } } ;
	LuaVal aaasds = { _K(123) = { 1,1,1 } };
	LuaVal asddf = { 1,2,3 };
	LuaVal dfdss = { 1,2 };
	LuaVal dfds = { _K(1)  = 2 };
	LuaVal fdsa = { 1 };

//	std::list<LuaVal> ret3 = LuaManager::instance()->call("Foo2", LuaVal{ _K("tab")  = 123 });
//	std::list<LuaVal> ret2 = LuaManager::instance()->call("Foo", 123, "abc", 1.1, LuaVal{ "tab",{1,2,3} }, LuaVal{ "mm",999 });

	LuaVal sdsadfsa = 1.1f;
	auto sdf1 = sdsadfsa.convert<bool>();
	auto sdf7 = sdsadfsa.convert<char>();
	auto sdf2 = sdsadfsa.convert<int>();
	auto sdf3 = sdsadfsa.convert<double>();
//	auto sdf4 = sdsadfsa.convert<const RefCountPtr<RefCountObj>&>();
//	auto sdf5 = sdsadfsa.convert<RefCountObj*>();
//	auto sdf6 = sdsadfsa.convert<RefCountPtr<RefCountObj>>();

	sdsadfsa = Object::create();
	auto sdf11 = sdsadfsa.convert<bool>();
//	auto sdf77 = sdsadfsa.convert<char>();
//	auto sdf22 = sdsadfsa.convert<int>();
//	auto sdf33 = sdsadfsa.convert<double>();
	auto sdf44 = sdsadfsa.convert<const RefCountPtr<RefCountObj>&>();
	auto sdf55 = sdsadfsa.convert<RefCountObj*>();
	auto sdf66 = sdsadfsa.convert<RefCountPtr<RefCountObj>>();

	LuaVal dasdas = LuaVal(true);
}
void TestFileSystem()
{
	Path path("D:\\dango\\document\\Visual Studio 2015\\Projects\\StarWay\\Engine\\TinyEngine");
	bool aaa = path.isDirectory();
	bool aaaa = path.isFile();
	bool aaaaa = path.isValid();
	auto list = path.getFileList();
	auto dsafdsa = path.getRelativePath();
	auto fsafdsa = path.getAbsolutePath();

	Path path2("game:Script");
	bool aaa2 = path2.isDirectory();
	bool aaaa2 = path2.isFile();
	bool aaaaa2 = path2.isValid();
	auto list2 = path2.getFileList();
	auto dsafdsa2 = path2.getRelativePath();
	auto fsafdsa2 = path2.getAbsolutePath();

	Path path3("game:/../..\\Tools/luac/src/../build/luac_32.exe");
	bool aaa3 = path3.isDirectory();
	bool aaaa3 = path3.isFile();
	bool aaaaa3 = path3.isValid();
	auto list3 = path3.getFileList();
	auto dsafdsa3 = path3.getRelativePath();
	auto fsafdsa3 = path3.getAbsolutePath();

	Path path4("game:../Script");
	bool aaa4 = path4.isDirectory();
	bool aaaa4 = path4.isFile();
	bool aaaaa4 = path4.isValid();
	auto list4 = path4.getFileList();
	auto dsafdsa4 = path4.getRelativePath();
	auto fsafdsa4 = path4.getAbsolutePath();

	Path path5("C:");
	bool aaa5 = path5.isDirectory();
	bool aaaa5 = path5.isFile();
	bool aaaaa5 = path5.isValid();
//	auto list5 = path5.getFileList();
	auto dsafdsa5 = path5.getRelativePath();
	auto fsafdsa5 = path5.getAbsolutePath();

//	File file;
//	file.open("E:\\a\\b\\112.txt");

	int a = 0;
}
void UnitTest()
{
	return;
	Common();
	TestMatrix();
	TestVector();
	TestQuaternion();
//	TestTimer();
	TestRect();
	TestLuaVal();
	TestFileSystem();
}