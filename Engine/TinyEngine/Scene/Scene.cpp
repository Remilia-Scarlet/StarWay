#include "TinyEngine/precomp.h"
#include "Scene.h"
#include "TinyEngine/Engine/Engine.h"
#include "Graphic/Manager/LightManager.h"
#include "TinyEngine/Components/DirectionLightComponent.h"
#include "TinyEngine/Components/PointLightComponet.h"
#include "Graphic/Manager/GraphicMgr.h"

bool Scene::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);
	LUA_PROTOTYPE_REGIST_FUN(addObject);
	LUA_PROTOTYPE_REGIST_FUN(removeObject);

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
	ScenePtr self = LuaManager::instance()->getVal<ScenePtr>(L, 1);
	ObjectPtr para1 = LuaManager::instance()->getVal<ObjectPtr>(L, 2);
	if (!self.isValid() || !para1.isValid())
		return LUA_PARAM_ERROR(Scene::L_addObject);
	self->addObject(para1);
	return 0;
}

void Scene::removeObject(ObjectPtr obj)
{
	if (obj.isValid())
	{
		_objects.erase(obj->getObjectId());
	}
}

int Scene::L_removeObject(lua_State* L)
{
	ScenePtr self = LuaManager::instance()->getVal<ScenePtr>(L, 1);
	ObjectPtr para1 = LuaManager::instance()->getVal<ObjectPtr>(L, 2);
	if (!self.isValid() || !para1.isValid())
		return LUA_PARAM_ERROR(Scene::L_removeObject);
	self->removeObject(para1);
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
	//build shadow map
	for (auto itLight = _lights.begin(); itLight != _lights.end(); ++itLight)
	{
		ObjectPtr obj = itLight->second.lock();
		if (!obj.isValid())
			continue;

		if (obj->getFlag(ObjectFlag::IS_DIRECTIONAL_LIGHT))
		{
			DirectionLightComponentPtr lightCom = obj->getComponent<DirectionLightComponent>();
			if (!lightCom.isValid())
				continue;

			bool result = lightCom->prepareRenderShadowMap();
			if (result)
			{
				for (auto itObj = _objects.begin(); itObj != _objects.end(); ++itObj)
				{
					if (itObj->second.isValid() && !(itObj->second->getFlag(ObjectFlag::IS_CAMERA) || itObj->second->getFlag(ObjectFlag::IS_LIGHT)))
					{
						itObj->second->render();
					}
				}
			}
			lightCom->finishedRenderShadowMap();
		}
		else if (obj->getFlag(ObjectFlag::IS_POINT_LIGHT))
		{
			PointLightComponetPtr lightCom = obj->getComponent<PointLightComponet>();
			if (!lightCom.isValid())
				continue;
		}
	}

	// render
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

			LightManager::instance()->render();

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
