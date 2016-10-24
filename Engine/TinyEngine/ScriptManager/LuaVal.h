#pragma once
#include "Ash\RefCountPointer\RefCountPtr.h"
#include "Ash\RefCountPointer\RefCountObj.h"
#include <memory>

class LuaVal
{
public:
	enum class DataType : int8_t
	{
		NIL,
		INT64,
		DOUBLE,
		STRING,
		REF_OBJ,
		TABLE
	};
	static const LuaVal NIL;

public:
	LuaVal();  // nil
	LuaVal(std::nullptr_t nil);  // nil
	LuaVal(bool b);  // boolean
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
	template <class T>
	LuaVal(const RefCountPtr<T>& obj);  // obj
	LuaVal(const LuaVal& other);  // copy
	LuaVal(LuaVal&& other);  // move

	~LuaVal();

	LuaVal& operator=(std::nullptr_t nil);  // nil
	LuaVal& operator=(bool b);  // boolean
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
	template <class T>
	LuaVal& operator=(const RefCountPtr<T>& obj);  // obj
	LuaVal& operator=(const LuaVal& other);  // copy
	LuaVal& operator=(LuaVal&& other);  // move

	bool operator==(const LuaVal& other) const;
public:
	void reset();  // nil
	void reset(std::nullptr_t nil);  // nil
	void reset(bool b);  // boolean
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
	template <class T>
	void reset(const RefCountPtr<T>& obj);  // obj
	void reset(const LuaVal& other);  // copy
	void reset(LuaVal&& other);  // move

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

	// Deep clone a LuaVal
	LuaVal clone() const;

	// For int, double and string, return true if the value is equal
	// For RefCountObj, compair there pointer
	// For table, compair the table pointer. That is, this->clone().euqal(this) == false
	bool equal(const LuaVal& other) const;
public:
	// create LuaVal from lua code. Example: LuaVal table = LuaVal::parse("{1,2,a=3,b={\"a\",\"b\",\"c\"}}");
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
	clear();
	_type = DataType::REF_OBJ;
	_data.obj = new RefCountPtr<RefCountObj>(obj);
}