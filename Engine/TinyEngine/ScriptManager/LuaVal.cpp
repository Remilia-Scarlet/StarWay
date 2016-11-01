#include "TinyEngine\precomp.h"
#include "LuaVal.h"
//////////////////////////////////////////////////////////////////////////
// Global fucntion
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Member fucntion
//////////////////////////////////////////////////////////////////////////
const LuaVal LuaVal::NIL;
std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc> LuaVal::s_getItWrongRet;

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

LuaVal::LuaVal(RefCountObj* obj)
{
	reset(obj);
}

LuaVal::LuaVal(std::initializer_list<_K> table)
{
	reset(table);
}

LuaVal::LuaVal(const LuaVal& other)
{
	reset(other);
}

LuaVal::LuaVal(LuaVal&& other)
{
	reset(std::move(other));
}


//LuaVal::LuaVal(const LuaVal& key, const LuaVal& val)
//{
//	int a = 0;
//}

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

LuaVal& LuaVal::operator=(RefCountObj* obj)
{
	reset(obj);
	return *this;
}

LuaVal& LuaVal::operator=(std::initializer_list<_K> table)
{
	reset(table);
	return *this;
}

LuaVal& LuaVal::operator=(const LuaVal& other)
{
	reset(other);
	return *this;
}

LuaVal& LuaVal::operator=(LuaVal&& other)
{
	reset(std::move(other));
	return *this;
}

bool LuaVal::operator==(const LuaVal& other) const
{
	return equal(other);
}

bool LuaVal::operator!=(const LuaVal& other) const
{
	return !equal(other);
}

LuaVal LuaVal::operator+(const LuaVal& other) const
{
	if (isInt64() && other.isInt64())
		return LuaVal(convertInt64() + other.convertInt64());

	if (isNumber() && other.isNumber())
		return LuaVal(convertDouble() + other.convertDouble());

	TinyAssert(false, "can't use operator+ for this two LuaVal");
	return LuaVal();
}

LuaVal LuaVal::operator-(const LuaVal& other) const
{
	if (isInt64() && other.isInt64())
		return LuaVal(convertInt64() - other.convertInt64());

	if (isNumber() && other.isNumber())
		return LuaVal(convertDouble() - other.convertDouble());

	TinyAssert(false, "can't use operator- for this two LuaVal");
	return LuaVal();
}

bool LuaVal::operator<(const LuaVal& other) const
{
	if (isNumber() && other.isNumber())
		return convertDouble() < other.convertDouble();

	if (_type != other._type)
		return (int)_type < (int)other._type;

	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return false;
	case LuaVal::DataType::BOOLEAN:
		return (int)_data.b < (int)other._data.b;
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		TinyAssert(false, "unreachable");
		break;
	case LuaVal::DataType::STRING:
		return std::strcmp(_data.s, other._data.s) < 0;
	case LuaVal::DataType::REF_OBJ:
		return *_data.obj < *other._data.obj;
	case LuaVal::DataType::TABLE:
		return *_data.table < *other._data.table;
	}
	TinyAssert(false, "unreachable");
	return false;
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
	_type = DataType::BOOLEAN;
	_data.b = b;
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
	if (_type == DataType::REF_OBJ && _data.obj->get() == &obj)
		return;

	clear();
	_type = DataType::REF_OBJ;
	_data.obj = new RefCountPtr<RefCountObj>(&obj);
}

void LuaVal::reset(RefCountObj* obj)
{
	if (_type == DataType::REF_OBJ && _data.obj->get() == obj)
		return;

	clear();
	_type = DataType::REF_OBJ;
	_data.obj = new RefCountPtr<RefCountObj>(obj);
}

void LuaVal::reset(std::initializer_list<_K> table)
{
	clear();

	int index = 1;
	_type = DataType::TABLE;
	_data.table = new std::shared_ptr<std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc> >(new std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>());
	std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>& dataTab = *((*_data.table).get());

	std::list<int64_t> usedKey;
	for (const _K& helper : table)
	{
		const LuaVal& key = helper._key;
		if (key.getType() == DataType::INT64)
			usedKey.push_back(key.convertInt64());
	}
	for (const _K& helper : table)
	{
		const LuaVal& key = helper._key;
		const LuaVal& val = helper._val;
		if (key.getType() == DataType::NIL)
		{
			auto it = usedKey.begin();
			while ((it = std::find(usedKey.begin(),usedKey.end(),index)) != usedKey.end())
				++index;
			dataTab[index++] = val;
		}
		else
			dataTab[key] = val;
	}
}

void LuaVal::reset(const LuaVal& other)
{
	if (this == &other)
		return;

	switch (other._type)
	{
	case LuaVal::DataType::NIL:
		reset(nullptr);
		break;
	case DataType::BOOLEAN:
		reset(other._data.b);
		break;
	case LuaVal::DataType::INT64:
		reset(other._data.i);
		break;
	case LuaVal::DataType::DOUBLE:
		reset(other._data.d);
		break;
	case LuaVal::DataType::STRING:
		reset(other._data.s);
		break;
	case LuaVal::DataType::REF_OBJ:
		reset(*other._data.obj);
		break;
	case LuaVal::DataType::TABLE:
		clear();
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
	case LuaVal::DataType::BOOLEAN:
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

LuaVal::DataType LuaVal::getType() const
{
	return _type;
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

LuaValTabIt LuaVal::begin()
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return LuaValTabIt(s_getItWrongRet.begin());
	}
	return LuaValTabIt(_data.table->get()->begin());
}

ConstLuaValTabIt LuaVal::begin() const
{
	return cbegin();
}

ConstLuaValTabIt LuaVal::cbegin() const
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return ConstLuaValTabIt(s_getItWrongRet.cbegin());
	}
	return ConstLuaValTabIt(_data.table->get()->cbegin());
}

LuaValTabIt LuaVal::end()
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return LuaValTabIt(s_getItWrongRet.end());
	}
	return LuaValTabIt(_data.table->get()->end());
}

ConstLuaValTabIt LuaVal::end() const
{
	return cend();
}

ConstLuaValTabIt LuaVal::cend() const
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return ConstLuaValTabIt(s_getItWrongRet.cend());
	}
	return ConstLuaValTabIt(_data.table->get()->cend());
}

std::string LuaVal::toString() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return "nil";
	case LuaVal::DataType::BOOLEAN:
		return _data.b ? "true" : "false";
	case LuaVal::DataType::INT64:
		return FormatString("%ld", _data.i);
	case LuaVal::DataType::DOUBLE:
		return FormatString("%.14g", _data.d);
	case LuaVal::DataType::STRING:
		return FormatString("\"%s\"",_data.s);
	case LuaVal::DataType::REF_OBJ:
		return FormatString("(Obj:%lX)", (int64_t)(*_data.obj).get());
	case LuaVal::DataType::TABLE:
		{
			std::map<LuaVal, LuaVal> sortedTable((*_data.table)->begin(), (*_data.table)->end()); //sort table
			std::string str = "{";
			bool first = true;
			for (auto pair : sortedTable)
			{
				if (first)
					first = false;
				else
					str += ",";
				const LuaVal& key = pair.first;
				const LuaVal& val = pair.second;
				str += getKeyString(key);
				str += "=";
				str += val.toString();
			}
			str += "}";
			return str;
		}
	}
	TinyAssert(false, "unreachable code");
	return "";
}

bool LuaVal::convertBoolean() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return false;
	case LuaVal::DataType::BOOLEAN:
		return _data.b;
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
		return true;
	default:
		TinyAssert(false, "can't convert LuaVal to boolean");
		break;
	}
	return false;
}

int32_t LuaVal::convertInt32() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return 0;
	case DataType::BOOLEAN:
		return _data.b ? 1 : 0;
	case LuaVal::DataType::INT64:
		return (int32_t)_data.i;
	case LuaVal::DataType::DOUBLE:
		return (int32_t)_data.d;
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to int32");
		break;
	}
	return 0;
}

int64_t LuaVal::convertInt64() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return 0;
	case DataType::BOOLEAN:
		return _data.b ? 1 : 0;
	case LuaVal::DataType::INT64:
		return _data.i;
	case LuaVal::DataType::DOUBLE:
		return (int64_t)_data.d;
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to int64");
		break;
	}
	return 0;
}

float LuaVal::convertFloat() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return 0.f;
	case DataType::BOOLEAN:
		return _data.b ? 1.f : 0.f;
	case LuaVal::DataType::INT64:
		return (float)_data.i;
	case LuaVal::DataType::DOUBLE:
		return (float)_data.d;
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to float");
		break;
	}
	return 0;
}

double LuaVal::convertDouble() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return 0.0;
	case DataType::BOOLEAN:
		return _data.b ? 1.0 : 0.0;
	case LuaVal::DataType::INT64:
		return (double)_data.i;
	case LuaVal::DataType::DOUBLE:
		return _data.d;
	case LuaVal::DataType::STRING:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to double");
		break;
	}
	return 0;
}

const char* LuaVal::convertCharPointer() const
{
	switch (_type)
	{
	case LuaVal::DataType::STRING:
		return _data.s;
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::BOOLEAN:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to char pointer");
		break;
	}
	return "";
}

std::string LuaVal::converString() const
{
	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return "";
	case DataType::BOOLEAN:
		return _data.b ? "true" : "false";
	case LuaVal::DataType::STRING:
		return _data.s;
	case LuaVal::DataType::INT64:
		return FormatString("%ld", _data.i);
	case LuaVal::DataType::DOUBLE:
		return FormatString("%.14g", _data.d);
	case LuaVal::DataType::REF_OBJ:
	case LuaVal::DataType::TABLE:
	default:
		TinyAssert(false, "can't convert LuaVal to string");
		break;
	}
	return "";
}

RefCountObj* LuaVal::convertRefObj() const
{
	if (_type == DataType::NIL)
		return nullptr;

	if (_type == DataType::REF_OBJ)
		return _data.obj->get();

	TinyAssert(false, "can't convert a LuaVal to RefObj");
	return nullptr;
}

bool LuaVal::isNumber() const
{
	return isInt64() || isDouble();
}

bool LuaVal::isNil() const
{
	return _type == DataType::NIL;
}

bool LuaVal::isBoolean() const
{
	return _type == DataType::BOOLEAN;
}

bool LuaVal::isInt64() const
{
	return _type == DataType::INT64;
}

bool LuaVal::isDouble() const
{
	return _type == DataType::DOUBLE;
}

bool LuaVal::isString() const
{
	return _type == DataType::STRING;
}

bool LuaVal::isRefObj() const
{
	return _type == DataType::REF_OBJ;
}

bool LuaVal::isTable() const
{
	return _type == DataType::TABLE;
}

LuaVal LuaVal::clone() const
{
	LuaVal val;
	switch (_type)
	{
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::BOOLEAN:
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
	if (_type == DataType::INT64 && other._type == DataType::INT64)
		return _data.i == other._data.i;

	if (isNumber() && other.isNumber())
		return isEqual(convertDouble(), other.convertDouble());

	if (_type != other._type)
		return false;

	switch (_type)
	{
	case LuaVal::DataType::NIL:
		return true;
	case LuaVal::DataType::BOOLEAN:
		return _data.b == other._data.b;
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		// unreachable
		break;
	case LuaVal::DataType::STRING:
		return strcmp(_data.s, other._data.s) == 0;
	case LuaVal::DataType::REF_OBJ:
		return _data.obj->get() == other._data.obj->get();
	case LuaVal::DataType::TABLE:
		return _data.table->get() == other._data.table->get();
	}
	TinyAssert(false, "unreachable code");
	return false;
}

void LuaVal::clear()
{
	switch (_type)
	{
	case DataType::NIL:
	case LuaVal::DataType::BOOLEAN:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		// base type, nothing to do
		break;
	case LuaVal::DataType::STRING:
		delete[] _data.s;
		break;
	case LuaVal::DataType::REF_OBJ:
		delete _data.obj;
		break;
	case LuaVal::DataType::TABLE:
		delete _data.table;
		break;
	}
	_data.i = 0;
	_type = DataType::NIL;
}

std::string LuaVal::getKeyString(const LuaVal& key) const
{
	switch (key._type)
	{
	case LuaVal::DataType::NIL:
		return "nil";
	case LuaVal::DataType::BOOLEAN:
		return _data.b ? "[true]" : "[false]";
	case LuaVal::DataType::INT64:
		return FormatString("[%ld]", key._data.i);
	case LuaVal::DataType::DOUBLE:
		return FormatString("[%.14g]", key._data.d);
	case LuaVal::DataType::STRING:
		if (key._data.s[0] == 0 || (key._data.s[0] > '0' && key._data.s[0] < '9'))
			return FormatString("[\"%s\"]", key._data.s);
		return key._data.s;
	case LuaVal::DataType::REF_OBJ:
		return FormatString("[(Obj:%lX)]", (int64_t)(*_data.obj).get());
	case LuaVal::DataType::TABLE:
		return FormatString("[(Table:%lX)]", (int64_t)(*_data.table).get());
	}
	TinyAssert(false, "unreachable code");
	return "";
}

_K::_K(_K&& other)
	:_key(std::move(other._key))
	,_val(std::move(other._val))
{
}

_K::_K(std::initializer_list<_K> table) : _key(LuaVal::NIL), _val(table)
{
}

_K::_K(std::nullptr_t nil) : _key(nullptr), _val(nullptr)
{
}

_K::_K(bool b) : _key(LuaVal::NIL), _val(b)
{
}

_K::_K(int8_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(uint8_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(int16_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(uint16_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(int32_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(uint32_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(int64_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(uint64_t i) : _key(LuaVal::NIL), _val(i)
{
}

_K::_K(float f) : _key(LuaVal::NIL), _val(f)
{
}

_K::_K(double d) : _key(LuaVal::NIL), _val(d)
{
}

_K::_K(const char* s) : _key(LuaVal::NIL), _val(s)
{
}

_K::_K(const std::string& s) : _key(LuaVal::NIL), _val(s)
{
}

_K::_K(RefCountObj& obj) : _key(LuaVal::NIL), _val(obj)
{
}

_K::_K(RefCountObj* obj) : _key(LuaVal::NIL), _val(obj)
{
}

_K&& _K::operator=(_K&& other)
{
	TinyAssert(_key.isNil() && other._key.isNil());
	_key = std::move(_val);
	_val = std::move(other._val);
	return std::move(*this);
}

LuaValTabIt LuaValTabIt::operator++()
{
	LuaValTabIt itCopy = *this;
	++_it;
	return itCopy;
}

LuaValTabIt& LuaValTabIt::operator++(int)
{
	++_it;
	return *this;
}

LuaValTabIt LuaValTabIt::operator--()
{
	LuaValTabIt itCopy = *this;
	--_it;
	return itCopy;
}

LuaValTabIt& LuaValTabIt::operator--(int)
{
	--_it;
	return *this;
}

std::pair<const LuaVal, LuaVal>* LuaValTabIt::operator->()
{
	return &(*_it);
}

std::pair<const LuaVal, LuaVal>& LuaValTabIt::operator*()
{
	return *_it;
}

LuaValTabIt::LuaValTabIt(const std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc>::iterator& it)
	:_it(it)
{
	
}

bool LuaValTabIt::operator!=(const LuaValTabIt& other) const
{
	return _it != other._it;
}

bool LuaValTabIt::operator==(const LuaValTabIt& other) const
{
	return _it == other._it;
}

LuaValTabIt& LuaValTabIt::operator=(const LuaValTabIt& other)
{
	_it = other._it;
	return *this;
}

ConstLuaValTabIt ConstLuaValTabIt::operator++()
{
	ConstLuaValTabIt itCopy = *this;
	++_it;
	return itCopy;
}

ConstLuaValTabIt& ConstLuaValTabIt::operator++(int)
{
	++_it;
	return *this;
}

ConstLuaValTabIt ConstLuaValTabIt::operator--()
{
	ConstLuaValTabIt itCopy = *this;
	--_it;
	return itCopy;
}

ConstLuaValTabIt& ConstLuaValTabIt::operator--(int)
{
	--_it;
	return *this;
}

const std::pair<const LuaVal, LuaVal>* ConstLuaValTabIt::operator->()
{
	return &(*_it);
}

const std::pair<const LuaVal, LuaVal>& ConstLuaValTabIt::operator*()
{
	return *_it;
}

ConstLuaValTabIt::ConstLuaValTabIt(const std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc>::const_iterator& it)
	:_it(it)
{

}

bool ConstLuaValTabIt::operator!=(const ConstLuaValTabIt& other) const
{
	return _it != other._it;
}

bool ConstLuaValTabIt::operator==(const ConstLuaValTabIt& other) const
{
	return _it == other._it;
}

ConstLuaValTabIt& ConstLuaValTabIt::operator=(const ConstLuaValTabIt& other)
{
	_it = other._it;
	return *this;
}