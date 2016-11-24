#include "TinyEngine/precomp.h"
#include "LuaFuncs.h"
#include "LuaManager.h"
#include <functional>
#include "TinyEngine/Scene/Scene.h"
#include "TinyEngine/Components/MeshComponent.h"
#include "TinyEngine/Other/GeometryGenerator.h"
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "TinyEngine/Components/TextureComponent.h"
#include "TinyEngine/Components/TransformComponent.h"
#include "TinyEngine/Components/CameraComponent.h"
#include "TinyEngine/Engine/Engine.h"
#include "TinyEngine/Components/ScriptComponent.h"
#include "TinyEngine/Components/DirectionLightComponent.h"
#include "TinyEngine/Input/InputManager.h"
#include "TinyEngine/Components/PointLightComponet.h"
#include "Math/vector/LuaVector.h"

LuaFuns* LuaFuns::instance()
{
	static LuaFuns instance;
	return &instance;
}

int LuaFuns::GenerateCubeMesh(lua_State* L)
{
	if(!lua_isnumber(L,1) || !lua_isnumber(L, 2) || !lua_isnumber(L, 3))
		return LUA_PARAM_ERROR(GenerateCubeMesh);

	float w = LuaManager::instance()->getVal<float>(L, 1);
	float h = LuaManager::instance()->getVal<float>(L, 2);
	float l = LuaManager::instance()->getVal<float>(L, 3);

	MeshComponentPtr mesh = GeometryGenerator::instance()->createCubeMesh(InputLayoutType::COMMON, w, h, l);
	LuaManager::instance()->pushVal(L, mesh);
	return 1;
}

int LuaFuns::GenerateSphereMesh(lua_State* L)
{
	if (!lua_isnumber(L, 1) || !lua_isnumber(L, 2))
		return LUA_PARAM_ERROR(GenerateSphereMesh);

	float radius = LuaManager::instance()->getVal<float>(L, 1);
	int numSubdivisions = LuaManager::instance()->getVal<int>(L, 2);

	MeshComponentPtr mesh = GeometryGenerator::instance()->createSphereMeshData(InputLayoutType::COMMON, radius, numSubdivisions);
	LuaManager::instance()->pushVal(L, mesh);
	return 1;
}

int LuaFuns::StartScene(lua_State* L)
{
	ScenePtr scene = LuaManager::instance()->getVal<ScenePtr>(L, 1);
	if(!scene.isValid())
		return LUA_PARAM_ERROR(StartScene);
	Engine::instance()->startScene(scene);
	return 0;
}

int LuaFuns::GetCurrentScene(lua_State* L)
{
	ScenePtr scene = Engine::instance()->getCurrentScene();
	LuaManager::instance()->pushVal(L, scene);
	return 1;
}

bool LuaFuns::registerFuncsToLua()
{
	// object
	Object::createLuaPrototype();
	Scene::createLuaPrototype();
	
	// component
	CameraComponent::createLuaPrototype();
	DirectionLightComponent::createLuaPrototype();
	MeshComponent::createLuaPrototype();
	ScriptComponent::createLuaPrototype();
	TextureComponent::createLuaPrototype();
	TransformComponent::createLuaPrototype();
	PointLightComponet::createLuaPrototype();
	LuaVector::createLuaPrototype();

	// singleton manager
	InputManager::createLuaPrototype();

	// global function
#define REGIST_FUN(_NAME_) do{lua_register(LuaManager::instance()->getLuaMachine(), #_NAME_, LuaFuns::_NAME_);}while(0)
	REGIST_FUN(GenerateCubeMesh);
	REGIST_FUN(GenerateSphereMesh);
	REGIST_FUN(StartScene);
	REGIST_FUN(GetCurrentScene);
#undef REGIST_FUN

	return true;
}

