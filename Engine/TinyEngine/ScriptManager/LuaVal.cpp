#include "TinyEngine\precomp.h"
#include "LuaVal.h"
#include "TinyEngine/Objects/Object.h"

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

LuaVal::~LuaVal()
{
	if(_type == DataType::REF_OBJ || _type == DataType::STRING || _type == DataType::TABLE)
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
		return *_data.str->get() < *other._data.str->get();
	case LuaVal::DataType::REF_OBJ:
		return *_data.obj < *other._data.obj;
	case LuaVal::DataType::TABLE:
		return *_data.tab->get() < *other._data.tab->get();
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
	_data.str = new std::shared_ptr<std::string>(new std::string(s));
}

void LuaVal::reset(const std::string& s)
{
	clear();
	_type = DataType::STRING;
	_data.str = new std::shared_ptr<std::string>(new std::string(s));
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

	_type = DataType::TABLE;
	_data.tab = new std::shared_ptr<TableSaver>(new TableSaver(false));
	std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>& dataTab = *_data.tab->get()->originTable;

	std::list<int64_t> usedKey;
	int index = 1;
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
			while ((it = std::find(usedKey.begin(), usedKey.end(), index)) != usedKey.end())
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
		reset(*other._data.str->get());
		break;
	case LuaVal::DataType::REF_OBJ:
		reset(*other._data.obj);
		break;
	case LuaVal::DataType::TABLE:
		if(_type == DataType::TABLE && *_data.tab == *other._data.tab)
			break;
		clear();
		_data.tab = new std::shared_ptr<TableSaver>(*other._data.tab);
		_type = DataType::TABLE;
		break;
	}
}

void LuaVal::reset(LuaVal&& other)
{
	if (this == &other)
		return;

	clear();
	_type = other._type;
	switch (_type)
	{
	case LuaVal::DataType::NIL:
	case LuaVal::DataType::BOOLEAN:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		_data.i = other._data.i;
		break;
	case LuaVal::DataType::STRING:
		_data.str = other._data.str;
		break;
	case LuaVal::DataType::REF_OBJ:
		_data.obj = other._data.obj;
		break;
	case LuaVal::DataType::TABLE:
		_data.tab = other._data.tab;
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
	const TableSaver& info = *_data.tab->get();
	if (info.isLuaRefTable)
	{
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, info.refTableId);
		TinyAssert(type == LUA_TTABLE);
		LuaManager::instance()->pushVal(key);
		lua_gettable(L, -2);
		LuaVal top = LuaManager::instance()->getVal(L, -1);
		lua_pop(L, 3);
		TinyAssert(oldTop == lua_gettop(L));
		return top;
	}
	else
	{
		auto it = info.originTable->find(key);
		if (it != info.originTable->end())
			return it->second;
		return NIL;
	}
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
		_data.tab = new std::shared_ptr<TableSaver>(new TableSaver(false));
	}
	const TableSaver& info = *_data.tab->get();
	if (info.isLuaRefTable)
	{
		lua_State* L = LuaManager::instance()->getLuaMachine();	
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, info.refTableId);
		TinyAssert(type == LUA_TTABLE);
		LuaManager::instance()->pushVal(key);
		LuaManager::instance()->pushVal(value);
		lua_settable(L, -3); 
		lua_pop(L, 3);
		TinyAssert(oldTop == lua_gettop(L));
	}
	else
	{
		std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>& table = *info.originTable;
		table[key] = value;
	}
	return *this;
}

LuaVal& LuaVal::setField(int index, const LuaVal& value)
{
	return setField(LuaVal(index),value);
}

int LuaVal::getLenth() const
{
	if (_type == DataType::STRING)
		return (int)(*_data.str)->length();
	else if (_type == DataType::TABLE)
	{
		if (_data.tab->get()->isLuaRefTable)
		{
			lua_State* L = LuaManager::instance()->getLuaMachine();
			int oldTop = lua_gettop(L);
			int type = lua_getglobal(L, LUAVAL_TABLE);
			TinyAssert(type == LUA_TTABLE);
			type = lua_rawgeti(L, -1, _data.tab->get()->refTableId);
			TinyAssert(type == LUA_TTABLE);
			int len = (int)luaL_len(L, -1);
			lua_pop(L, 2);
			TinyAssert(oldTop == lua_gettop(L));
			return len;
		}
		else
			return (int)(_data.tab->get()->originTable->size());
	}
	else
	{
		TinyAssert(false);
		return 0;
	}
}

LuaValTabIt LuaVal::begin() const
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return LuaValTabIt(s_getItWrongRet.begin());
	}
	if ((*_data.tab)->isLuaRefTable)
		return ++LuaValTabIt((*_data.tab)->refTableId);
	else
		return LuaValTabIt((*_data.tab)->originTable->begin());
}

LuaValTabIt LuaVal::end() const
{
	if (_type != DataType::TABLE)
	{
		TinyAssert(false, "It's not table");
		return LuaValTabIt(s_getItWrongRet.end());
	}
	if ((*_data.tab)->isLuaRefTable)
		return LuaValTabIt(-(*_data.tab)->refTableId);
	else
		return LuaValTabIt((*_data.tab)->originTable->end());
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
		return FormatString("\"%s\"", (*_data.str)->c_str());
	case LuaVal::DataType::REF_OBJ:
		return FormatString("(Obj:%lX)", (int64_t)(*_data.obj).get());
	case LuaVal::DataType::TABLE:
	{
		if (isLuaRefTable())
		{
			lua_State* L = LuaManager::instance()->getLuaMachine();
			int top = lua_gettop(L);
			int type = lua_getglobal(L, LUAVAL_TABLE);
			TinyAssert(type == LUA_TTABLE);
			type = lua_rawgeti(L, -1, getLuaRefTableId());
			TinyAssert(type == LUA_TTABLE);
			std::string ret = FormatString("(Table:%lX)", (int64_t)lua_topointer(L, -1));
			lua_pop(L, 2);
			TinyAssert(top == lua_gettop(L));
			return ret;
		}
		else
		{
			std::map<LuaVal, LuaVal> sortedTable; //sort table
			for (auto it = begin(); it != end(); ++it)
				sortedTable[it.key()] = it.val();
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
	default:
		break;
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
		return (*_data.str)->c_str();
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
		return *_data.str->get();
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
		val._data.tab = new std::shared_ptr<TableSaver>(_data.tab->get()->clone());
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
		return *_data.str->get() == *other._data.str->get();
	case LuaVal::DataType::REF_OBJ:
		return _data.obj->get() == other._data.obj->get();
	case LuaVal::DataType::TABLE:
		return *_data.tab == *other._data.tab;
	}
	TinyAssert(false, "unreachable code");
	return false;
}

void LuaVal::clear()
{
	switch (_type)
	{
	case DataType::NIL:
		return;
	case LuaVal::DataType::BOOLEAN:
	case LuaVal::DataType::INT64:
	case LuaVal::DataType::DOUBLE:
		// base type, nothing to do
		break;
	case LuaVal::DataType::STRING:
		delete _data.str;
		break;
	case LuaVal::DataType::REF_OBJ:
		delete _data.obj;
		break;
	case LuaVal::DataType::TABLE:
		delete _data.tab;
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
	{
		const std::string& str = *key._data.str->get();
		if (str.length() == 0 || (str.at(0) > '0' && str.at(0) < '9'))
			return FormatString("[\"%s\"]", str.c_str());
		return str;
	}
	case LuaVal::DataType::REF_OBJ:
		return FormatString("[(Obj:%lX)]", (int64_t)(*_data.obj).get());
	case LuaVal::DataType::TABLE:
		return FormatString("[(Table:%lX)]", ((*_data.tab)->isLuaRefTable ? _data.tab->get()->refTableId : (int64_t)(*_data.tab)->originTable));
	}
	TinyAssert(false, "unreachable code");
	return "";
}

void LuaVal::setRefTable(int64_t refTableId)
{
	if (_type == DataType::TABLE && _data.tab->get()->refTableId == refTableId)
		return;

	clear();
	_type = DataType::TABLE;
	_data.tab = new std::shared_ptr<TableSaver>(new TableSaver(true));
	_data.tab->get()->refTableId = refTableId;
}

bool LuaVal::isLuaRefTable() const
{
	return _type == DataType::TABLE && _data.tab->get()->isLuaRefTable;
}

int64_t LuaVal::getLuaRefTableId() const
{
	if (!isLuaRefTable())
		return 0;
	return _data.tab->get()->refTableId;
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

LuaValTabIt LuaValTabIt::operator++(int)
{
	LuaValTabIt itCopy = *this;
	++(*this);
	return itCopy;
}

LuaValTabIt& LuaValTabIt::operator++()
{
	if (_isRefTable)
	{
		if (_tableId <= 0)
		{
			TinyAssert(false, "This iterator is ended!");
			return *this;
		}
		++_stepIndex;
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, _tableId);
		TinyAssert(type == LUA_TTABLE);
		LuaManager::instance()->pushVal(_key);
		int result = lua_next(L, -2);
		if (result == 0)
		{
			_tableId = -_tableId;
			lua_pop(L, 2);
		}
		else
		{
			_key = LuaManager::instance()->getVal(L, -2);
			_val = LuaManager::instance()->getVal(L, -1);
			lua_pop(L, 4);
		}
		TinyAssert(oldTop == lua_gettop(L));
	}
	else
	{
		++_it;
	}
	return *this;
}

//LuaValTabIt LuaValTabIt::operator--()
//{
//	LuaValTabIt itCopy = *this;
//	--_it;
//	return itCopy;
//}
//
//LuaValTabIt& LuaValTabIt::operator--(int)
//{
//	if (_isRefTable)
//	{
//		// lua unsupport
//	}
//	else
//	{
//		--_it;
//	}
//	return *this;
//}

const LuaVal LuaValTabIt::key() const
{
	if (_isRefTable)
		return _key;
	else
		return _it->first;
}

const LuaVal LuaValTabIt::val() const
{
	if (_isRefTable)
		return _val;
	else
		return _it->second;
}
LuaValTabIt::LuaValTabIt(const std::unordered_map<LuaVal, LuaVal, LuaVal::HashFunc, LuaVal::CmpFunc>::iterator& it)
	: _it(it)
	, _tableId(0)
	, _isRefTable(false)
	, _stepIndex(0)
{

}

LuaValTabIt::LuaValTabIt(int64_t tableId)
	: _tableId(tableId)
	, _isRefTable(true)
	, _stepIndex(0)
{

}

bool LuaValTabIt::operator!=(const LuaValTabIt& other) const
{
	return !(*this == other);
}

bool LuaValTabIt::operator==(const LuaValTabIt& other) const
{
	if (_isRefTable != other._isRefTable)
		return false;

	if (_isRefTable)
	{
		if (_tableId != other._tableId)
			return false;
		return _stepIndex == other._stepIndex || _tableId < 0;
	}
	else
		return _it == other._it;
}

LuaValTabIt& LuaValTabIt::operator=(const LuaValTabIt& other)
{
	if (*this == other)
		return *this;

	_isRefTable = other._isRefTable;
	_it = other._it;
	_tableId = other._tableId;
	_key = other._key;
	_val = other._val;
	_stepIndex = other._stepIndex;

	return *this;
}

size_t LuaVal::HashFunc::operator()(const LuaVal& key) const
{
	switch (key._type)
	{
	case DataType::NIL:
		return 99999999;
	case DataType::BOOLEAN:
		return key._data.b ? 1234567 : 7654321;
	case DataType::INT64:
		return std::hash<int64_t>()(key._data.i);
	case DataType::DOUBLE:
		return std::hash<float>()(float(key._data.d));
	case DataType::STRING:
		return std::hash<std::string>()(*key._data.str->get());
	case DataType::REF_OBJ:
		return std::hash<void*>()(key._data.obj->get());
	case DataType::TABLE:
		if ((*key._data.tab)->isLuaRefTable)
			return std::hash<int64_t>()((*key._data.tab)->refTableId);
		else
			return std::hash<void*>()((*key._data.tab)->originTable);
	default:
		return 0;
	}
}

LuaVal::TableSaver::TableSaver(TableSaver&& other)
{
	refTableId = other.refTableId;
	isLuaRefTable = other.isLuaRefTable;
	other.refTableId = 0;
	other.isLuaRefTable = true;
}

LuaVal::TableSaver::TableSaver(bool isLuaRefTable)
	:isLuaRefTable(isLuaRefTable)
{
	if (isLuaRefTable)
		refTableId = 0;
	else
		originTable = new std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>();
}

LuaVal::TableSaver::~TableSaver()
{
	if (isLuaRefTable && refTableId)
	{
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		lua_pushinteger(L, refTableId);
		lua_pushnil(L);
		lua_settable(L, -3);
		lua_pop(L, 1);
		TinyAssert(oldTop == lua_gettop(L));
	}
	else if (!isLuaRefTable && originTable)
	{
		delete originTable;
	}
}

bool LuaVal::TableSaver::operator==(const TableSaver& other) const
{
	if (isLuaRefTable != other.isLuaRefTable)
		return false;

	if (isLuaRefTable)
	{
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, refTableId);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -2, other.refTableId);
		TinyAssert(type == LUA_TTABLE);
		int result = lua_compare(L, -1, -2, LUA_OPEQ);
		lua_pop(L, 3);
		TinyAssert(oldTop == lua_gettop(L));
		return result == 0 ? false : true;
	}
	else
	{
		return originTable == other.originTable;
	}
}

bool LuaVal::TableSaver::operator<(const TableSaver& other) const
{
	if (isLuaRefTable != other.isLuaRefTable)
		return int(isLuaRefTable) < int(other.isLuaRefTable);

	if (isLuaRefTable)
	{
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, refTableId);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -2, other.refTableId);
		TinyAssert(type == LUA_TTABLE);
		int result = lua_compare(L, -2, -1, LUA_OPLT);
		lua_pop(L, 4);
		TinyAssert(oldTop == lua_gettop(L));
		return result ? true : false;
	}
	else
		return originTable < other.originTable;
}

std::shared_ptr<LuaVal::TableSaver> LuaVal::TableSaver::clone() const
{
	TableSaver* ptr = new TableSaver(isLuaRefTable);
	if (isLuaRefTable)
	{
		ptr->isLuaRefTable = false;
		ptr->originTable = new std::unordered_map<LuaVal, LuaVal, HashFunc, CmpFunc>();
		lua_State* L = LuaManager::instance()->getLuaMachine();
		int oldTop = lua_gettop(L);
		int type = lua_getglobal(L, LUAVAL_TABLE);
		TinyAssert(type == LUA_TTABLE);
		type = lua_rawgeti(L, -1, refTableId);
		TinyAssert(type == LUA_TTABLE);

		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			LuaVal key = LuaManager::instance()->getVal(L, -2);
			LuaVal val = LuaManager::instance()->getVal(L, -1);
			(*ptr->originTable)[std::move(key)] = std::move(val);
		}
		lua_pop(L, 2);
		TinyAssert(oldTop == lua_gettop(L));
	}
	else
	{
		*ptr->originTable = *originTable;
	}
	return std::shared_ptr<LuaVal::TableSaver>(ptr);
}

