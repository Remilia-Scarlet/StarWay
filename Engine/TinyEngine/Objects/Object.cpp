#include "TinyEngine\precomp.h"
#include "Object.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "TinyEngine\Components\BaseComponent.h"
#include "TinyEngine\Components\CameraComponent.h"
#include "Graphic\DX11\DX11ConstantBufferManager.h"


ObjectPtr Object::create()
{
	Object* ret = new Object();
	if (!ret || !ret->init())
		TINY_SAFE_DELETE(ret);
	ObjectPtr ptr(ret);
	return ptr;
}

int Object::L_create(lua_State* L)
{
	ObjectPtr ret = Object::create();
	LuaManager::instance()->pushVal(ret);
	return 1;
}

void Object::addChild(ObjectPtr child)
{
	if (!child.isValid())
		return;
	ensureChildMap();
	(*_children)[child->getObjectId()] = child;
	child->setParent(ObjectPtr(this));
}

void Object::removeChild(ObjectPtr child)
{
	TinyAssert(false, "Not Support yet");
}


void Object::addComponent(const BaseComponentPtr& component)
{
	if (component.isValid())
	{
		ensureComponentMap();
		(*_components)[component->getObjectId()] = component;
		if (DynamicRefCountCast<CameraComponent>(component).isValid())
			setFlag(ObjectFlag::IS_CAMERA, true);
		component->setOwner(RefCountPtr<Object>(this));
	}
	else
		TinyAssert(false,"Object::addComponent ptr is not valid");
}

int Object::L_addComponent(lua_State* L)
{
	LuaVal obj = LuaManager::getVal(L, 1);
	LuaVal component = LuaManager::getVal(L, 2);
	if (!obj.isRefObj() || !component.isRefObj())
		return LUA_PARAM_ERROR(AddComponent);
	ObjectPtr objPtr = obj.convertRefPtr_dynamic<Object>();
	BaseComponentPtr compoPtr = component.convertRefPtr_dynamic<BaseComponent>();
	if (!objPtr.isValid() || !compoPtr.isValid())
		return LUA_PARAM_ERROR(AddComponent);
	objPtr->addComponent(compoPtr);
	return 0;
}

BaseComponentPtr Object::getComponent(ObjectID componentId)
{
	if (_components)
	{
		auto it = _components->find(componentId);
		if (it != _components->end())
			return it->second;
	}
	return BaseComponentPtr();
}

void Object::update(float dt)
{
	//@@TODO : if in update, _components was removed a item, it will crash
	if (_components)
	{
		for (auto it = _components->begin(); it != _components->end(); ++it)
			it->second->update(dt);
	}
}

void Object::render()
{
	GraphicMgr::instance()->setDrawIndexNumber(0);
	if (_components)
	{
		for (auto it = _components->begin(); it != _components->end(); ++it)
			it->second->render();
	}
	DX11ConstantBufferManager::instance()->commitVSBuffer();
	DX11ConstantBufferManager::instance()->commitPSBuffer();
	GraphicMgr::instance()->draw();

	if (_children)
	{
		for (auto it = _children->begin(); it != _children->end(); ++it)
			it->second->render();
	}
}

void Object::setFlag(ObjectFlag flagType, bool val)
{
	_flag[static_cast<size_t>(flagType)] = val;
}

bool Object::getFlag(ObjectFlag flagType)
{
	return _flag[static_cast<size_t>(flagType)];
}


bool Object::createLuaObj()
{
	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldSize = lua_gettop(L);
	lua_getglobal(L, CPP_LUA_POTABLE);
	lua_pushinteger(L, lua_Integer(this));

	lua_newtable(L);
	lua_getglobal(L, TO_STRING(Object));
	lua_setmetatable(L, -2);

	lua_pushstring(L, LUA_CPP_REF_NAME);
	lua_pushlightuserdata(L, this);
	lua_rawset(L, -3);

	lua_rawset(L, -3);
	lua_pop(L, 1);
	TinyAssert(oldSize == lua_gettop(L));
	return true;
}

void Object::ensureChildMap()
{
	if (!_children)
		_children = new std::unordered_map<ObjectID, ObjectPtr>();
}

void Object::ensureComponentMap()
{
	if (!_components)
		_components = new std::unordered_map<ObjectID, BaseComponentPtr>();
}

bool Object::init()
{
	do 
	{
		TINY_BREAK_IF(!createLuaObj());
		return true;
	} while (0);
	return false;
}

Object::Object()
	:_id(genericObjectId())
{
}

Object::~Object()
{
	TINY_SAFE_DELETE(_components);
	TINY_SAFE_DELETE(_children);
}

void Object::setParent(ObjectPtr parent)
{
	_parent = parent;
}

bool Object::createLuaPrototype()
{
	PROTOTYPE_PREPARE();

	PROTOTYPE_REGISTER_FUN(create);
	PROTOTYPE_REGISTER_FUN(addComponent);

	PROTOTYPE_END(Object);
	return true;
}