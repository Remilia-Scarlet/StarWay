#pragma once

//////////////////////////////////////////////////////////////////////////
// Useage:
// You can construct a LuaVal from these ways:
// Number:
// LuaVal val = 1;    //int
// val = 2.2;         //double
//
// Boolean:
// val = true;	      //bool
//
// string:
// val = "abc";       //const char*
// val = std::string("abc"); //std::string
// 
// RefCountObj:
// Notice : LuaVal will add strong ref count
// ObjectPtr obj = Object::create();					// strong ref == 1
// LuaVal val1 = obj;				// RefCountPtr		// strong ref == 2
// LuaVal val2 = obj.get();			// RefCountObj*		// strong ref == 3
// LuaVal val3 = *obj.get();		// RefCountObj&		// strong ref == 4
//
// table:
// You can construct table with key-val pair.
// {_K(key) = val} is like lua {[key] = val}
// For example if you want to construct a table like lua table:
// {[1] = 999, [2] = "abc", ["x"] = 123, ["y"] = 456, ["tab"] = {[1] = 1,[2] = 2,[3] = 3} }// Notice : in lua, table index starts from 1 not 0.
// You can construct LuaVal like this:
// val = {
//		_K(1) = 999,
//		_K(2) = "abc",
//		_K("x") = 123,
//		_K("y") = 456,
//		_K("tab") = {_K(1) = 1,_K(2) = 2,_K(3) = 3}
// };
// Just same as lua table, it can be simplified.
// The upper lua table can be simplified as : {999,"abc",x=123,y=456,tab={1,2,3}}
// Also, LuaVal can be construct as : val = {999,"abc",_K("x") = 123, _K("y") = 456, _K("tab") = {1,2,3} }

// Notice : the copy constructor and operator== of LuaVal table are shadow copy.
// For example, you construct a new LuaVal:
// LuaVal shadowCopy = val;
// Now, shadowCopy reference to the same table with val.
// If you call shadowCopy.setField("x",999);
// The "x" field of both val and shadowCopy will become 999.
// If you want deep copy, call clone()
// LuaVal deepCopy = shadowCopy.clone();
// Now your modification on deepCopy will not affect shadowCopy and val
// LuaVal string is treated as basical type. You don't need to worry about shadow copy and deep copy.
//////////////////////////////////////////////////////////////////////////

#include "TinyEngine\ThirdParty\lua_5_3_3\lua.hpp"
#include "Ash\RefCountPointer\RefCountPtr.h"
#include "Ash\RefCountPointer\RefCountObj.h"
#include <memory>
#include <unordered_map>

struct _K;
class LuaValTabIt;
class ConstLuaValTabIt;
class LuaManager;

class LuaVal
{
	friend class LuaValTabIt;
	friend class LuaManager;
public:
	enum class DataType : int8_t
	{
		NIL,
		BOOLEAN,
		INT64,
		DOUBLE,
		STRING,
		REF_OBJ,
		TABLE,
		USER_DATA
	};
	static const LuaVal NIL;

public:
	LuaVal();  // nil
	LuaVal(std::nullptr_t nil);  // nil
	explicit LuaVal(bool b);//boolean. It's explicit to avoid bool default convertion
	LuaVal(int8_t i);  // int
	LuaVal(uint8_t i);  // int
	LuaVal(int16_t i);  // int
	LuaVal(uint16_t i);  // int
	LuaVal(int32_t i);  // int
	LuaVal(uint32_t i);  // int
	LuaVal(int64_t i);  // int
	LuaVal(uint64_t i);  // int
	LuaVal(float f);  // number
	LuaVal(double d);  // number
	LuaVal(const char* s);  // string
	LuaVal(const std::string& s);  // string
	LuaVal(RefCountObj& obj);  // obj
	LuaVal(RefCountObj* obj);  // obj
	template <class T>
	LuaVal(const RefCountPtr<T>& obj);  // obj
	LuaVal(std::initializer_list<_K> table);//table
	LuaVal(const LuaVal& other);  // copy
	LuaVal(LuaVal&& other);  // move

	~LuaVal();

	LuaVal& operator=(std::nullptr_t nil);  // nil
	LuaVal& operator=(bool b);		//boolean
	LuaVal& operator=(int8_t i);  // int
	LuaVal& operator=(uint8_t i);  // int
	LuaVal& operator=(int16_t i);  // int
	LuaVal& operator=(uint16_t i);  // int
	LuaVal& operator=(int32_t i);  // int
	LuaVal& operator=(uint32_t i);  // int
	LuaVal& operator=(int64_t i);  // int
	LuaVal& operator=(uint64_t i);  // int
	LuaVal& operator=(float f);  // number
	LuaVal& operator=(double d);  // number
	LuaVal& operator=(const char* s);  // string
	LuaVal& operator=(const std::string& s);  // string
	LuaVal& operator=(RefCountObj& obj);  // obj
	LuaVal& operator=(RefCountObj* obj);  // obj
	template <class T>
	LuaVal& operator=(const RefCountPtr<T>& obj);  // obj
	LuaVal& operator=(std::initializer_list<_K> table);//table
	LuaVal& operator=(const LuaVal& other);  // copy
	LuaVal& operator=(LuaVal&& other);  // move

	bool operator==(const LuaVal& other) const;
	bool operator!=(const LuaVal& other) const;
	LuaVal operator+(const LuaVal& other) const;
	LuaVal operator-(const LuaVal& other) const;
	bool operator<(const LuaVal& other) const;
public:
	void reset();  // nil
	void reset(std::nullptr_t nil);  // nil
	void reset(bool b);			//boolean
	void reset(int8_t i);  // int
	void reset(uint8_t i);  // int
	void reset(int16_t i);  // int
	void reset(uint16_t i);  // ints
	void reset(int32_t i);  // int
	void reset(uint32_t i);  // int
	void reset(int64_t i);  // int
	void reset(uint64_t i);  // int
	void reset(float f);  // number
	void reset(double d);  // number
	void reset(const char* s);  // string
	void reset(const std::string& s);  // string
	void reset(RefCountObj& obj);  // obj
	void reset(RefCountObj* obj);  // obj
	template <class T>
	void reset(const RefCountPtr<T>& obj);  // obj
	void reset(std::initializer_list<_K> table);//table
	void reset(const LuaVal& other);  // copy
	void reset(LuaVal&& other);  // move

	// return type
	DataType getType() const;

	// Get field by name. Return LuaVal::NIL if find no field.
	LuaVal getField(const LuaVal& key) const;

	// Get field by index. Return LuaVal::NIL if find no field. Index starts from 1.
	LuaVal getField(int index) const;

	// Convert type to table, and set field by name. Return this.
	LuaVal& setField(const LuaVal& key, const LuaVal& value);

	// Convert type to table, and set field by index. Return this.
	LuaVal& setField(int index, const LuaVal& value);

	// Get lenth. Only aviliable on table and string
	int getLenth() const;

	// only aviliable for Table
	LuaValTabIt begin() const;

	// only aviliable for Table
	LuaValTabIt end() const;

	// get describe of this LuaVal
	std::string toString() const;

	/* convert LuaVal to a certain type. If convertion can not be done, an assert will be given, and a default value will be returned.*/
	bool			convertBoolean() const;//Like all tests in Lua, convertBoolean() returns true for any Lua value different from false and nil; otherwise it returns false.
	int32_t			convertInt32() const; //low 32 bit of this int64 value
	int64_t			convertInt64() const;
	float			convertFloat() const;
	double			convertDouble() const;
	const char*		convertCharPointer() const; // Point to inner char array. Use it soon.
	std::string		converString() const;
	RefCountObj*	convertRefObj() const; // Only when type is NIL or REF_OBJ can call this function
	template<class T>
	RefCountPtr<T>	convertRefPtr_static() const; // Only when type is NIL or REF_OBJ can call this function. StaticCast to T pointer
	template<class T>
	RefCountPtr<T>	convertRefPtr_dynamic() const; // Only when type is NIL or REF_OBJ can call this function. DynamicCast to T pointer
	void*			convertUserData() const;
	template<class T>
	typename std::remove_reference<typename T>::type convert() const;

	
	bool isNumber() const;// to check if type is INT64 || DOUBLE
	bool isNil() const; // to check if type is NIL
	bool isBoolean() const;// to check if type is BOOLEAN
	bool isInt64() const;// to check if type is INT64
	bool isDouble() const;// to chekc if type is DOUBLE
	bool isString() const; // to check if type is STRING
	bool isRefObj() const; // to check if type is REF_OBJ
	bool isTable() const; // to check if type is TABLE
	bool isUserData() const; // to check if type is USER_DATA

	// Deep clone a LuaVal
	LuaVal clone() const;

	// For int, double and string, return true if the value is equal
	// For RefCountObj, compair their pointers
	// For table, compair the table pointer. That is, this->clone().euqal(this) == false
	bool equal(const LuaVal& other) const;
public:
	// create LuaVal from lua code. Example: LuaVal table = LuaVal::parse("{999,123,a=3,b={\"a\",\"b\",\"c\"}}");
	// will construct a LuaVal like 
	// {
	//		{1,999},
	//		{2,123},
	//		{"a",3},
	//		{"b",{1,2,3}}
	// }
	static LuaVal parse(const char* luaCode);
	static LuaVal parse(const std::string& luaCode);

protected:
	void clear();
	std::string getKeyString(const LuaVal& key) const;
	void setRefTable(int64_t refTableId);
	void setUserData(int64_t refTableId);
	bool isLuaRefTable() const;
	int64_t getLuaRefTableId() const;
protected:
	struct HashFunc
	{
		size_t operator()(const LuaVal& key) const;
	};
	struct CmpFunc
	{
		bool operator()(const LuaVal& a, const LuaVal& b) const
		{
			return a.equal(b);
		}
	};
	struct TableSaver
	{
		TableSaver(bool isLuaRefTable);
		TableSaver(const TableSaver& other) = delete;
		TableSaver(TableSaver&& other);
		~TableSaver();
		bool operator==(const TableSaver& other) const;
		bool operator<(const TableSaver& other) const;
		std::shared_ptr<TableSaver> clone() const;
		
		union 
		{
			std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>* originTable;
			int64_t refTableId;
		};
		bool isLuaRefTable;
	};
	union Data
	{
		Data() { 
			static_assert(sizeof(Data) == sizeof(int64_t), "wrong!");
			i = 0;
		}
		~Data() {}
		int64_t i;
		bool b;
		double d;
		std::shared_ptr<std::string>* str;
		RefCountPtr<RefCountObj>* obj;
		std::shared_ptr<TableSaver>* tab;
	} _data;
	DataType _type = DataType::NIL;


	static std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc> s_getItWrongRet;
};

template <class T>
LuaVal::LuaVal(const RefCountPtr<T>& obj)
{
	reset(obj);
}

template <class T>
LuaVal& LuaVal::operator=(const RefCountPtr<T>& obj)
{
	reset(obj);
	return *this;
}

template <class T>
void LuaVal::reset(const RefCountPtr<T>& obj)
{
	if (_type == DataType::REF_OBJ && (*_data.obj) == obj)
		return;

	clear();
	_type = DataType::REF_OBJ;
	_data.obj = new RefCountPtr<RefCountObj>(obj.get());
}


template<class T>
RefCountPtr<T> LuaVal::convertRefPtr_static() const
{
	if (_type == DataType::NIL)
		return RefCountPtr<T>();

	if (_type == DataType::REF_OBJ)
		return StaticRefCountCast<T>(*_data.obj);

	TinyAssert(false, "can't convert a LuaVal to RefPtr");
	return RefCountPtr<T>();
}

template<class T>
RefCountPtr<T> LuaVal::convertRefPtr_dynamic() const
{
	if (_type == DataType::NIL)
		return RefCountPtr<T>();

	if (_type == DataType::REF_OBJ)
		return DynamicRefCountCast<T>(*_data.obj);

	TinyAssert(false, "can't convert a LuaVal to RefPtr");
	return RefCountPtr<T>();
}

template<int T,class TAR>
struct LuaValConverter
{
	static const LuaVal& convert(const LuaVal& val) { return val; }
};

template<class TAR>
struct LuaValConverter<int(LuaVal::DataType::BOOLEAN), TAR>
{
	static bool convert(const LuaVal& val) { return val.convertBoolean(); }
};

template<class TAR>
struct LuaValConverter<int(LuaVal::DataType::INT64), TAR>
{
	static int64_t convert(const LuaVal& val) { return val.convertInt64(); }
};

template<class TAR>
struct LuaValConverter<int(LuaVal::DataType::STRING), TAR>
{
	static const char* convert(const LuaVal& val) { return val.convertCharPointer(); }
};

template<class TAR>
struct LuaValConverter<int(LuaVal::DataType::DOUBLE), TAR>
{
	static double convert(const LuaVal& val) { return val.convertDouble(); }
};

template<class TAR>
struct LuaValConverter<int(LuaVal::DataType::REF_OBJ), TAR>
{
	static typename GetRefPtrInner<typename std::remove_cv<typename std::remove_reference<typename TAR>::type>::type>::type convert(const LuaVal& val)
	{
		return typename GetRefPtrInner<typename std::remove_cv<typename std::remove_reference<typename TAR>::type>::type>::type(val.convertRefObj());
	}
};

template<class T>
typename std::remove_reference<typename T>::type LuaVal::convert() const
{
	constexpr bool isBool = std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type, bool>::value;
	constexpr bool isFloatPoint = std::is_floating_point<typename T>::value;
	constexpr bool isIntegral = std::is_integral<typename T>::value;
	constexpr bool isString = std::is_same<typename T, const char*>::value || std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type,std::string>::value;
	constexpr bool isPointer = std::is_pointer<typename GetRefPtrInner<std::remove_cv<std::remove_reference<typename T>::type>::type>::type>::value;

	constexpr int type =
		(isBool ? (int)LuaVal::DataType::BOOLEAN
			: (isString ? (int)LuaVal::DataType::STRING
				: (isFloatPoint ? (int)LuaVal::DataType::DOUBLE
					: (isIntegral ? (int)LuaVal::DataType::INT64
						: (isPointer ? (int)LuaVal::DataType::REF_OBJ
							: -1 )
						)
					)
				)
			);

	return std::remove_reference<T>::type(LuaValConverter<type, T>::convert(*this));
}

struct _K
{
	friend class LuaVal;
public:
	_K()= delete;
	_K(const _K& other)= delete;
	_K(_K&& other);
	_K(std::initializer_list<_K> table);
	_K(std::nullptr_t nil);  // nil
	_K(bool b);  // boolean
	_K(int8_t i);  // int
	_K(uint8_t i);  // int
	_K(int16_t i);  // int
	_K(uint16_t i);  // int
	_K(int32_t i);  // int
	_K(uint32_t i);  // int
	_K(int64_t i);  // int
	_K(uint64_t i);  // int
	_K(float f);  // number
	_K(double d);  // number
	_K(const char* s);  // string
	_K(const std::string& s);  // string
	_K(RefCountObj& obj);  // obj
	_K(RefCountObj* obj);  // obj
	template <class T>
	_K(const RefCountPtr<T>& obj);  // obj

	_K&& operator=(_K&& other);
	_K& operator=(const _K& other) = delete;
private:
	LuaVal _key;
	LuaVal _val;
};

template <class T>
_K::_K(const RefCountPtr<T>& obj)
	: _key(LuaVal::NIL), _val(obj)
{

}

class LuaValTabIt
{
	friend class LuaVal;
public:
	LuaValTabIt operator++(int);
	LuaValTabIt& operator++();
//	LuaValTabIt operator--();
//	LuaValTabIt& operator--(int);
	const LuaVal key() const;
	const LuaVal val() const;
	bool operator==(const LuaValTabIt& other) const;
	bool operator!=(const LuaValTabIt& other) const;
	LuaValTabIt& operator=(const LuaValTabIt& other);
protected:
	LuaValTabIt() = delete;
	LuaValTabIt(const std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc>::iterator& it);
	LuaValTabIt(int64_t tableId);

	bool _isRefTable;
	std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc>::iterator _it;
	int64_t _tableId;
	LuaVal _key;
	LuaVal _val;
	int32_t _stepIndex;
};
