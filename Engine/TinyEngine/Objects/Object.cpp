#include "TinyEngine\precomp.h"
#include "Object.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "TinyEngine\Components\BaseComponent.h"

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

void Object::render()
{
	if (_components)
	{
		for (auto it = _components->begin(); it != _components->end(); ++it)
			it->second->render();
	}
	if (_children)
	{
		for (auto it = _children->begin(); it != _children->end(); ++it)
			it->second->render();
	}
	GraphicMgr::instance()->draw();
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
