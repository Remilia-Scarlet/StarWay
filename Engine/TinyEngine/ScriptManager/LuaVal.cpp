#include "TinyEngine\precomp.h"
#include "LuaVal.h"

const LuaVal LuaVal::NIL;

LuaVal::LuaVal()
{
}

LuaVal::LuaVal(std::nullptr_t nil)
{
}

LuaVal::LuaVal(bool b)
{
	reset(b);
}

LuaVal::LuaVal(int8_t i)
{
	reset(i);
}

LuaVal::LuaVal(uint8_t i)
{
	reset(i);
}

LuaVal::LuaVal(int16_t i)
{
	reset(i);
}

LuaVal::LuaVal(uint16_t i)
{
	reset(i);
}

LuaVal::LuaVal(int32_t i)
{
	reset(i);
}

LuaVal::LuaVal(uint32_t i)
{
	reset(i);
}

LuaVal::LuaVal(int64_t i)
{
	reset(i);
}

LuaVal::LuaVal(uint64_t i)
{
	reset(i);
}

LuaVal::LuaVal(float f)
{
	reset(f);
}

LuaVal::LuaVal(double d)
{
	reset(d);
}

LuaVal::LuaVal(const char* s)
{
	reset(s);
}

LuaVal::LuaVal(const std::string& s)
{
	reset(s);
}

LuaVal::LuaVal(RefCountObj& obj)
{
	reset(obj);
}

LuaVal::LuaVal(const LuaVal& other)
{
	reset(other);
}

LuaVal::LuaVal(LuaVal&& other)
{
	reset(other);
}

LuaVal::~LuaVal()
{
	clear();
}

LuaVal& LuaVal::operator=(std::nullptr_t nil)
{
	reset();
	return *this;
}

LuaVal& LuaVal::operator=(bool b)
{
	reset(b);
	return *this;
}

LuaVal& LuaVal::operator=(int8_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(uint8_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(int16_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(uint16_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(int32_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(uint32_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(int64_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(uint64_t i)
{
	reset(i);
	return *this;
}

LuaVal& LuaVal::operator=(float f)
{
	reset(f);
	return *this;
}

LuaVal& LuaVal::operator=(double d)
{
	reset(d);
	return *this;
}

LuaVal& LuaVal::operator=(const char* s)
{
	reset(s);
	return *this;
}

LuaVal& LuaVal::operator=(const std::string& s)
{
	reset(s);
	return *this;
}

LuaVal& LuaVal::operator=(RefCountObj& obj)
{
	reset(obj);
	return *this;
}

LuaVal& LuaVal::operator=(const LuaVal& other)
{
	reset(other);
	return *this;
}

LuaVal& LuaVal::operator=(LuaVal&& other)
{
	reset(other);
	return *this;
}

bool LuaVal::operator==(const LuaVal& other) const
{
	return equal(other);
}

void LuaVal::reset()
{
	clear();
}

void LuaVal::reset(std::nullptr_t nil)
{
	clear();
}

void LuaVal::reset(bool b)
{
	clear();
	_type = DataType::INT64;
	_data.i = (b ? 1 : 0);
}

void LuaVal::reset(int8_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(uint8_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(int16_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(uint16_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(int32_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(uint32_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(int64_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = i;
}

void LuaVal::reset(uint64_t i)
{
	clear();
	_type = DataType::INT64;
	_data.i = (int64_t)i;
}

void LuaVal::reset(float f)
{
	clear();
	_type = DataType::DOUBLE;
	_data.d = (double)f;
}

void LuaVal::reset(double d)
{
	clear();
	_type = DataType::DOUBLE;
	_data.d = d;
}

void LuaVal::reset(const char* s)
{
	clear();
	_type = DataType::STRING;
	auto len = strlen(s);
	_data.s = new char[len + 1];
	memcpy(_data.s, s, len + 1);
}

void LuaVal::reset(const std::string& s)
{
	clear();
	_type = DataType::STRING;
	auto len = s.length();
	_data.s = new char[len + 1];
	memcpy(_data.s, s.c_str(), len);
	_data.s[len] = 0;
}

void LuaVal::reset(RefCountObj& obj)
{
	clear();
	_type = DataType::REF_OBJ;
	_data.obj = new RefCountPtr<RefCountObj>(&obj);
}

void LuaVal::reset(const LuaVal& other)
{
	if (this == &other)
		return;

	switch (other._type)
	{
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		reset(other._data.i);
		break;
	case LuaVal::DataType::STRING:
		reset(other._data.s);
		break;
	case LuaVal::DataType::REF_OBJ:
		reset(*other._data.obj);
		break;
	case LuaVal::DataType::TABLE:
		_data.table = new std::shared_ptr<std::unordered_map<LuaVal,LuaVal,HashFunc,CmpFunc> >(*other._data.table);
		_type = DataType::TABLE;
		break;
	}
}

void LuaVal::reset(LuaVal&& other)
{
	if (this == &other)
		return;

	clear();
	_type = other._type;;
	switch (_type)
	{
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		_data.i = other._data.i;
		break;
	case LuaVal::DataType::STRING:
		_data.s = other._data.s;
		break;
	case LuaVal::DataType::REF_OBJ:
		_data.obj = other._data.obj;
		break;
	case LuaVal::DataType::TABLE:
		_data.table = other._data.table;
		break;
	}
	other._data.i = 0;
	other._type = DataType::NIL;
}

LuaVal LuaVal::getField(const LuaVal& key) const
{
	if (_type != DataType::TABLE)
		return NIL;
	auto it = (*_data.table)->find(key);
	if (it != (*_data.table)->end())
		return it->second;
	return NIL;
}

LuaVal LuaVal::getField(int index) const
{
	return getField(LuaVal(index));
}

LuaVal& LuaVal::setField(const LuaVal& key, const LuaVal& value)
{
	if (_type != DataType::TABLE)
	{
		clear();
		_type = DataType::TABLE;
		_data.table = new std::shared_ptr<std::unordered_map<LuaVal,LuaVal,HashFunc,CmpFunc> >(new std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>());
	}
	std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>* table = (*_data.table).get();
	(*table)[key] = value;
	return *this;
}

LuaVal& LuaVal::setField(int index, const LuaVal& value)
{
	return setField(LuaVal(index),value);
}

std::string LuaVal::toString() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return "nil";
		break;
	case LuaVal::DataType::INT64:
		return FormatString("%ld", _data.i);
		break;
	case LuaVal::DataType::DOUBLE:
		return FormatString("%.14g", _data.d);
		break;
	case LuaVal::DataType::STRING:
		return _data.s;
		break;
	case LuaVal::DataType::REF_OBJ:
		return FormatString("(Obj:%lX)", (int64_t)(*_data.obj).get());
		break;
	case LuaVal::DataType::TABLE:
		{
			std::string str = "{";
			std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>& table = *(*_data.table).get();
			bool first = true;
			for (auto pair : table)
			{
				if (first)
					first = false;
				else
					str += ",";
				const LuaVal& key = pair.first;
				const LuaVal& val = pair.second;
				str += getKeyString(key);
				str += "=";
				str += getValString(val);
			}
			str += "}";
		}
		break;
	}
	TinyAssert(false, "unreachable code");
	return "";
}

LuaVal LuaVal::clone() const
{
	LuaVal val;
	switch (_type)
	{
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
		val.reset(*this);
		break;
	case LuaVal::DataType::TABLE:
		val._type = _type;
		val._data.table = new std::shared_ptr<std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc> >(
			new std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>(
				*((*_data.table).get())
				)
			);
		break;
	}
	return val;
}

bool LuaVal::equal(const LuaVal& other) const
{
	if (_type != other._type)
		return false;
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return true;
	case LuaVal::DataType::INT64:
		return _data.i == other._data.i;
	case LuaVal::DataType::DOUBLE:
		return isEqual(_data.d, other._data.d);
	case LuaVal::DataType::STRING:
		return strcmp(_data.s, other._data.s) == 0;
	case LuaVal::DataType::REF_OBJ:
		return _data.obj->get() == other._data.obj->get();
	case LuaVal::DataType::TABLE:
		return _data.table->get() == other._data.table->get();
	}
	TinyAssert(false, "unreachable code");
	return "";
}

void LuaVal::clear()
{
	switch (_type)
	{
	case DataType::NIL:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		// base type, nothing to do
		break;
	case LuaVal::DataType::STRING:
		delete[] _data.s;
		break;
	case LuaVal::DataType::REF_OBJ:
		_data.obj->~RefCountPtr();
		break;
	case LuaVal::DataType::TABLE:
		_data.table->~shared_ptr();
		break;
	}
	_data.i = 0;
	_type = DataType::NIL;
}

std::string LuaVal::getKeyString(const LuaVal& key) const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return "nil";
		break;
	case LuaVal::DataType::INT64:
		return FormatString("[%ld]", key._data.i);
		break;
	case LuaVal::DataType::DOUBLE:
		return FormatString("[%.14g]", key._data.d);
		break;
	case LuaVal::DataType::STRING:
		return key._data.s;
		break;
	case LuaVal::DataType::REF_OBJ:
		return FormatString("[(Obj:%lX)]", (int64_t)(*_data.obj).get());
		break;
	case LuaVal::DataType::TABLE:
		return FormatString("[(Table:%lX)]", (int64_t)(*_data.table).get());
		break;
	}
	TinyAssert(false, "unreachable code");
	return "";
}

std::string LuaVal::getValString(const LuaVal& val) const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return "nil";
		break;
	case LuaVal::DataType::INT64:
		return FormatString("%ld", val._data.i);
		break;
	case LuaVal::DataType::DOUBLE:
		return FormatString("%.14g", val._data.d);
		break;
	case LuaVal::DataType::STRING:
		return FormatString("\"%s\"", val._data.s);
		break;
	case LuaVal::DataType::REF_OBJ:
		return FormatString("(Obj:%lX)", (int64_t)(*_data.obj).get());
		break;
	case LuaVal::DataType::TABLE:
		return val.toString();
		break;
	}
	TinyAssert(false, "unreachable code");
	return "";
}

