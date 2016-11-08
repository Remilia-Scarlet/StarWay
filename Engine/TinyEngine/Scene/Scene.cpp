#include "TinyEngine/precomp.h"
#include "Scene.h"
#include "TinyEngine/Engine/Engine.h"

bool Scene::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);
	LUA_PROTOTYPE_REGIST_FUN(addObject);

	LUA_PROTOTYPE_END(Scene);
	return true;
}

Scene::Scene()
{
	LUA_GENERATE_OBJ(TO_STRING(Scene));
}

Scene::~Scene()
{
	LUA_REMOVE_OBJ();
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
		if (obj->getFlag(ObjectFlag::IS_LIGHT))
			_lights[obj->getObjectId()] = obj;
	}

}

int Scene::L_addObject(lua_State* L)
{
	ScenePtr self = LuaManager::instance()->getVal(L, 1).convertRefPtr_dynamic<Scene>();
	ObjectPtr para1 = LuaManager::instance()->getVal(L, 2).convertRefPtr_dynamic<Object>();
	if (!self.isValid() || !para1.isValid())
		return LUA_PARAM_ERROR(Scene::L_addObject);
	self->addObject(para1);
	return 0;
}

void Scene::update(float dt)
{
	//@@TODO : if in update, _object removed a item, it will crash
	for (auto it = _objects.begin(); it != _objects.end(); ++it)
	{
		if (it->second.isValid())
		{
			it->second->update(dt);
		}
	}
}

void Scene::render()
{
	for (auto itCamera = _cameras.begin(); itCamera != _cameras.end();)
	{
		if (itCamera->second.isValid())
		{
			ObjectPtr camera = itCamera->second.lock();
			++itCamera;
			if(!camera->getEnable())
				continue;
			camera->render();

			for (auto itLight = _lights.begin(); itLight != _lights.end();)
			{
				if (itLight->second.isValid())
				{
					itLight->second.lock()->render();
					++itLight;
				}
				else
					itLight = _lights.erase(itLight);
			}


			for (auto itObj = _objects.begin(); itObj != _objects.end(); ++itObj)
			{
				if (itObj->second.isValid() && !(itObj->second->getFlag(ObjectFlag::IS_CAMERA) || itObj->second->getFlag(ObjectFlag::IS_LIGHT)))
				{
					itObj->second->render();
				}
			}
		}
		else
			itCamera = _cameras.erase(itCamera);
	}
}


bool Scene::init()
{
	return true;
}
