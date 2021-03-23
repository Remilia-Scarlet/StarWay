#include "DangoEngine/precomp.h"
#include "Object.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "DangoEngine/Components/BaseComponent.h"
#include "DangoEngine/Components/CameraComponent.h"
#include "Graphic/DX11/DX11ConstantBufferManager.h"
#include "DangoEngine/Engine/Engine.h"
#include "DangoEngine/Components/DirectionLightComponent.h"
#include "DangoEngine/Components/PointLightComponet.h"
#include "Ash/MultiThread/Task.h"

ObjectPtr Object::create()
{
	Object* ret = new Object();
	if (!ret || !ret->init())
		TINY_SAFE_DELETE(ret);
	ObjectPtr ptr(ret);
	return ptr;
}

ObjectPtr Object::create(const std::string& objName)
{
	Object* ret = new Object();
	if (!ret || !ret->init(objName))
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
		component->setOwner(RefCountPtr<Object>(this));
	}
	else
		TinyAssert(false,"Object::addComponent ptr is not valid");
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

void Object::update(Task* task, float dt)
{
	//@@TODO : if in update, _components removed a item, it will crash
	if (_components)
	{
		for (auto it = _components->begin(); it != _components->end(); ++it)
		{
			TaskPtr compTaskPtr = MakeRefCountPtr<Task>(std::bind(&BaseComponent::update, it->second.get(), std::placeholders::_1, dt));
			task->addNextTask(compTaskPtr);
		}
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

int Object::getFlags()
{
	return (int)_flag.to_ulong();
}

void Object::setEnable(bool enable)
{
	_enable = enable;
}

bool Object::getEnable()
{
	return _enable;
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
	std::string name = FormatString("Obj__%ld", getObjectId());
	return init(name);
}

bool Object::init(const std::string& objName)
{
	do
	{
		_name = objName;
		return true;
	} while (0);
	return false;
}

Object::Object()
	:_enable(true)
	,_children(nullptr)
	,_components(nullptr)
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
