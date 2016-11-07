#include "Graphic/precomp.h"
#include "LightManager.h"
#include "ConstantBufferManager.h"

LightManager* LightManager::s_instance = nullptr;

bool LightManager::createInstance()
{
	TinyAssert(s_instance == nullptr, "Memory leaks! Call destroyInstance() first!");
	s_instance = new LightManager();
	if (!s_instance || !s_instance->init())
		TINY_SAFE_DELETE(s_instance);
	return s_instance != nullptr;
}

void LightManager::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

LightManager* LightManager::instance()
{
	TinyAssert(s_instance != nullptr, "No instance!! Call createInstance() first!");
	return s_instance;
}

void LightManager::applyDirectionLight(const DirectionLight& directionLight)
{
	if (_lightInfo.lightNumber.X() < (float)MAX_LIGHT)
	{
		_lightInfo.directionalLight[int(_lightInfo.lightNumber.X())] = directionLight;
		++_lightInfo.lightNumber.X();
	}
}

void LightManager::preRender()
{
	_lightInfo.lightNumber.reset();
}

void LightManager::render()
{
	ConstantBufferManager::instance()->setRegistPSBuffer(1, &_lightInfo);
}

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
	
}

bool LightManager::init()
{
	do 
	{
		ConstantBufferManager::instance()->registPSBuffer(1, sizeof(LightInfo));
		return true;
	} while (0);
	TinyAssert(false, "LightManager creating failed!");
	return false;
}
