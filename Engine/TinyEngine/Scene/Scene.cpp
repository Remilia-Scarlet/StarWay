#include "TinyEngine/precomp.h"
#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

ScenePtr Scene::create()
{
	Scene* ret = new Scene();
	if (!ret || !ret->init())
	{
		TINY_SAFE_DELETE(ret);
		return ScenePtr();
	}
	return ScenePtr(ret);
}

void Scene::addObject(ObjectPtr obj)
{
	if (obj.isValid())
	{
		_objects[obj->getObjectId()] = obj;
		if (obj->getFlag(ObjectFlag::IS_CAMERA))
			_cameras[obj->getObjectId()] = obj;
	}

}

void Scene::render()
{
	for (auto it = _cameras.begin(); it != _cameras.end();)
	{
		if (it->second.isValid())
		{
			it->second.lock()->render();
			++it;
		}
		else
			it = _cameras.erase(it);
	}

	for (auto it = _objects.begin(); it != _objects.end(); ++it)
	{
		if (it->second.isValid() && !it->second->getFlag(ObjectFlag::IS_CAMERA))
		{
			it->second->render();
		}
	}
}


bool Scene::init()
{
	return true;
}
