#include "TinyEngine\precomp.h"
#include "Object.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "TinyEngine\Components\BaseComponent.h"
#include "TinyEngine\Components\CameraComponent.h"
#include "Graphic\DX11\DX11ConstantBufferManager.h"
#include "TinyEngine\Engine\Engine.h"


bool Object::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);
	LUA_PROTOTYPE_REGIST_FUN(addComponent);
	LUA_PROTOTYPE_REGIST_FUN(getComponent);

	LUA_PROTOTYPE_END(Object);
	return true;
}

ObjectPtr Object::create()
{
	Object* ret = new Object();
	if (!ret || !ret->init())
		TINY_SAFE_DELETE(ret);
	ObjectPtr ptr(ret);
	return ptr;
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
LUA_MEMBER_FUN_P1R0_IMPL(Object, addComponent, const BaseComponentPtr&)
//
//int Object::L_addComponent(lua_State* L)
//{
//	ObjectPtr objPtr = LuaManager::instance()->getVal(L, 1).convertRefPtr_dynamic<Object>();
//	BaseComponentPtr compoPtr = LuaManager::instance()->getVal(L, 2).convertRefPtr_dynamic<BaseComponent>();
//	if (!objPtr.isValid() || !compoPtr.isValid())
//		return LUA_PARAM_ERROR(AddComponent);
//	objPtr->addComponent(compoPtr);
//	return 0;
//}

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

BaseComponentPtr Object::getComponent(const std::string& name)
{
	if (_components)
	{
		for (auto& pair : *_components)
		{
			if (pair.second->getName() == name)
				return pair.second;
		}
	}
	return BaseComponentPtr();
}

int Object::L_getComponent(lua_State* L)
{
	Object* ooo = nullptr;
	RefCountObj* fdsa = ooo;
	ObjectPtr self = LuaManager::instance()->getVal(L, 1).convertRefPtr_dynamic<Object>();
	LuaVal componentName = LuaManager::instance()->getVal(L, 2);
	if (!self.isValid() || !componentName.isString())
		return LUA_PARAM_ERROR(Object::L_getComponent);

	LuaManager::instance()->pushVal(L, self->getComponent(componentName.convertCharPointer()));
	return 1;
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
		return true;
	} while (0);
	return false;
}

Object::Object()
	:_id(genericObjectId())
{
	LUA_GENERATE_OBJ(TO_STRING(Object));
}

Object::~Object()
{
	TINY_SAFE_DELETE(_components);
	TINY_SAFE_DELETE(_children);
	LUA_REMOVE_OBJ();
}

void Object::setParent(ObjectPtr parent)
{
	_parent = parent;
}
