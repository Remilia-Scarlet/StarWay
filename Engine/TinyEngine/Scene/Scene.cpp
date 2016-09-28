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
	}

}

void Scene::render()
{
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
	{
		if (it->second.isValid())
		{
			it->second->render();
		}
	}
}


bool Scene::init()
{
	return true;
}
