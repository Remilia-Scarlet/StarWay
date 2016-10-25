#pragma once

//////////////////////////////////////////////////////////////////////////
// Useage:
// You can construct a LuaVal from these ways:
// number:
// LuaVal val = 1;    //int
// val = 2.2;         //double
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
// Notice : in lua, table index starts from 1 not 0.
// val = {
//		{1, 1},
//		{2, "abc"},
//		{"x", 123},
//		{"y", 456},
//		{"tab",{1,2,3}}
// };
// This is the same as lua table {1,"abc",x=123,y=456,tab={1,2,3}}
// Index of table is not needed if it increase from 1. The table is the same as below:
// val = {
//		1,
//		"abc",
//		{"x", 123},
//		{"y", 456},
//		{"tab",{1,2,3}}
// };
// Notice : the copy constructor and operator== of LuaVal is shadow copy.
// For example, you construct a new LuaVal:
// LuaVal shadowCopy = val;
// Now, shadowCopy reference to the same table with val.
// If you call shadowCopy.setField("x",999);
// The "x" field of both val and shadowCopy will become 999.
// If you want deep copy, call clone()
// LuaVal deepCopy = shadowCopy.clone();
// Now your modification on deepCopy will not affect shadowCopy and val
//////////////////////////////////////////////////////////////////////////

#include "Ash\RefCountPointer\RefCountPtr.h"
#include "Ash\RefCountPointer\RefCountObj.h"
#include <memory>
#include <unordered_map>
struct LuaTableConstructHelper;
class LuaVal
{
public:
	enum class DataType : int8_t
	{
		NIL = 1,
		BOOLEAN=2,
		INT64 = 4,
		DOUBLE = 8,
		STRING = 16,
		REF_OBJ = 32,
		TABLE = 64
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
	LuaVal(std::initializer_list<LuaTableConstructHelper> table);//table
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
	LuaVal& operator=(std::initializer_list<LuaTableConstructHelper> table);//table
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
	void reset(std::initializer_list<LuaTableConstructHelper> table);//table
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

	// get describe of this LuaVal
	std::string toString() const;

	/* convert LuaVal to a certain type. If convertion can not be done, an assert will be given, and a default value will be returned.*/
	int32_t			convertInt32() const; //low 32 bit of this int64 value
	int64_t			convertInt64() const;
	float			convertFloat() const;
	double			convertDouble() const;
	const char*		convertCharPointer() const; // Point to inner char array. Use it soon.
	std::string		converString() const;
	RefCountObj*	convertRefObj() const; // Only when type is NIL or REF_OBJ can call this function
	template<class T>
	RefCountPtr<T>	convertRefPtr() const; // Only when type is NIL or REF_OBJ can call this function. 

	
	bool isNumber() const;// to check if type is INT64 || DOUBLE
	bool isNil() const; // to check if type is NIL
	bool isBoolean() const;// to check if type is BOOLEAN
	bool isInt64() const;// to check if type is INT64
	bool isDouble() const;// to chekc if type is DOUBLE
	bool isString() const; // to check if type is STRING
	bool isRefObj() const; // to check if type is REF_OBJ
	bool isTable() const; // to check if type is TABLE

	// Deep clone a LuaVal
	LuaVal clone() const;

	// For int, double and string, return true if the value is equal
	// For RefCountObj, compair there pointer
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
	std::string getValString(const LuaVal& val) const;
protected:
	struct HashFunc
	{
		size_t operator()(const LuaVal& key) const
		{
			return (size_t)(int64_t(key._type) ^ key._data.i);
		}
	};
	struct CmpFunc
	{
		bool operator()(const LuaVal& a, const LuaVal& b) const
		{
			return a.equal(b);
		}
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
		char* s;
		RefCountPtr<RefCountObj>* obj;
		std::shared_ptr<std::unordered_map<LuaVal,LuaVal,HashFunc,CmpFunc> >* table;
	} _data;
	DataType _type = DataType::NIL;
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
	_data.obj = new RefCountPtr<RefCountObj>(obj);
}


template<class T>
RefCountPtr<T> LuaVal::convertRefPtr() const
{
	if (_type == DataType::NIL)
		return nullptr;

	if (_type == DataType::REF_OBJ)
		return RefCountPtr<T>(_data.obj->get());

	TinyAssert(false, "can't convert a LuaVal to RefPtr");
	return nullptr;
}

struct LuaTableConstructHelper
{
	LuaTableConstructHelper(const LuaVal& key, const LuaVal& val);
	LuaTableConstructHelper(std::nullptr_t nil);  // nil
	LuaTableConstructHelper(bool b);  // boolean
	LuaTableConstructHelper(int8_t i);  // int
	LuaTableConstructHelper(uint8_t i);  // int
	LuaTableConstructHelper(int16_t i);  // int
	LuaTableConstructHelper(uint16_t i);  // int
	LuaTableConstructHelper(int32_t i);  // int
	LuaTableConstructHelper(uint32_t i);  // int
	LuaTableConstructHelper(int64_t i);  // int
	LuaTableConstructHelper(uint64_t i);  // int
	LuaTableConstructHelper(float f);  // number
	LuaTableConstructHelper(double d);  // number
	LuaTableConstructHelper(const char* s);  // string
	LuaTableConstructHelper(const std::string& s);  // string
	LuaTableConstructHelper(RefCountObj& obj);  // obj
	LuaTableConstructHelper(RefCountObj* obj);  // obj
	template <class T>
	LuaTableConstructHelper(const RefCountPtr<T>& obj);  // obj

	LuaVal _key;
	LuaVal _val;
};

template <class T>
LuaTableConstructHelper::LuaTableConstructHelper(const RefCountPtr<T>& obj)
	: _key(LuaVal::NIL), _val(obj)
{

}
