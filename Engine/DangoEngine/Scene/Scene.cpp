#include "DangoEngine/precomp.h"
#include "Scene.h"
#include "DangoEngine/Engine/Engine.h"
#include "Graphic/Manager/LightManager.h"
#include "DangoEngine/Components/DirectionLightComponent.h"
#include "DangoEngine/Components/PointLightComponet.h"
#include "Graphic/Manager/GraphicMgr.h"
#include "Ash/MultiThread/Task.h"

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
		if (obj->getFlag(ObjectFlag::IS_LIGHT))
			_lights[obj->getObjectId()] = obj;
	}

}

void Scene::removeObject(ObjectPtr obj)
{
	if (obj.isValid())
	{
		_objects.erase(obj->getObjectId());
	}
}

void Scene::update(Ash::FunctorSeq& seq, float dt)
{
	std::vector<Ash::Functor> funcs;
	funcs.reserve(_objects.size());
	for(auto& it : _objects)
	{
		funcs.emplace_back(std::bind(&Object::update, it.second.get(), std::placeholders::_1, dt));
	}
	seq.then(std::move(funcs));
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
