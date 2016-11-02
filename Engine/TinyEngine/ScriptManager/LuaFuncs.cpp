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


#define REGIST_FUN(_NAME_) do{lua_register(LuaManager::instance()->getLuaMachine(), #_NAME_, LuaFuns::_NAME_);}while(0)
#define LUA_PARAM_ERROR(_NAME_) luaL_error(L, "function %s param error", #_NAME_)

LuaFuns* LuaFuns::instance()
{
	static LuaFuns instance;
	return &instance;
}

int LuaFuns::CreateScene(lua_State* L)
{
	ScenePtr scene = Scene::create();
	pushVal(L, scene);
	return 1;
}

int LuaFuns::CreateObject(lua_State* L)
{
	ObjectPtr obj = Object::create();
	pushVal(L, obj);
	return 1;
}

int LuaFuns::CreateTexture(lua_State* L)
{
	LuaVal texture = getVal(L, 1);
	LuaVal shader = getVal(L, 2);
	if (!texture.isString() || !shader.isString())
		return LUA_PARAM_ERROR(CreateTexture);
	TextureComponentPtr texCom = TextureComponent::create(texture.converString(), shader.converString());
	pushVal(L, texCom);
	return 1;
}

int LuaFuns::CreateTransform(lua_State* L)
{
	TransformComponentPtr transform = TransformComponent::create();
	pushVal(L, transform);
	return 1;
}

int LuaFuns::CreateCamera(lua_State* L)
{
	CameraComponentPtr ptr = CameraComponent::create();
	pushVal(L, ptr);
	return 1;
}

int LuaFuns::CreateScriptComponent(lua_State* L)
{
	LuaVal tableName = getVal(L, 1);
	if(!tableName.isString())
		return LUA_PARAM_ERROR(CreateScriptComponent);
	ScriptComponentPtr script = ScriptComponent::create(tableName.converString());
	pushVal(L, script);
	return 1;
}

int LuaFuns::SetLocation(lua_State* L)
{
	LuaVal transform = getVal(L, 1);
	TransformComponentPtr ptr = transform.convertRefPtr_dynamic<TransformComponent>();
	if(transform.isNil() || !ptr.isValid())
		return LUA_PARAM_ERROR(SetLocation);
	LuaVal x = getVal(L, 2);
	LuaVal y = getVal(L, 3);
	LuaVal z = getVal(L, 4);
	if(!x.isNumber() || !y.isNumber() || !z.isNumber())
		return LUA_PARAM_ERROR(SetLocation);
	ptr->setLocation(x.convertFloat(), y.convertFloat(), z.convertFloat());
	return 0;
}

int LuaFuns::SetRotation(lua_State* L)
{
	LuaVal transform = getVal(L, 1);
	TransformComponentPtr ptr = transform.convertRefPtr_dynamic<TransformComponent>();
	if (transform.isNil() || !ptr.isValid())
		return LUA_PARAM_ERROR(SetRotation);
	LuaVal x = getVal(L, 2);
	LuaVal y = getVal(L, 3);
	LuaVal z = getVal(L, 4);
	if (!x.isNumber() || !y.isNumber() || !z.isNumber())
		return LUA_PARAM_ERROR(SetRotation);
	ptr->setRotation(x.convertFloat(), y.convertFloat(), z.convertFloat());
	return 0;
}

int LuaFuns::FaceToPoint(lua_State* L)
{
	LuaVal transform = getVal(L, 1);
	TransformComponentPtr ptr = transform.convertRefPtr_dynamic<TransformComponent>();
	if (transform.isNil() || !ptr.isValid())
		return LUA_PARAM_ERROR(FaceToPoint);
	LuaVal x = getVal(L, 2);
	LuaVal y = getVal(L, 3);
	LuaVal z = getVal(L, 4);
	if (!x.isNumber() || !y.isNumber() || !z.isNumber())
		return LUA_PARAM_ERROR(FaceToPoint);
	ptr->faceToPoint({ x.convertFloat(), y.convertFloat(), z.convertFloat() });
	return 0;
}

int LuaFuns::AddObject(lua_State* L)
{
	ScenePtr scene = getVal(L, 1).convertRefPtr_dynamic<Scene>();
	ObjectPtr obj = getVal(L, 2).convertRefPtr_dynamic<Object>();
	if(!scene.isValid() || !obj.isValid())
		return LUA_PARAM_ERROR(AddObject);
	scene->addObject(obj);
	return 0;
}

int LuaFuns::GenerateCubeMesh(lua_State* L)
{
	LuaVal w = getVal(L, 1);
	LuaVal h = getVal(L, 2);
	LuaVal l = getVal(L, 3);
	if (!w.isNumber() || !h.isNumber() || !l.isNumber())
		return LUA_PARAM_ERROR(GenerateCubeMesh);
	MeshComponentPtr mesh = GeometryGenerator::instance()->createCubeMesh(InputLayoutType::COMMON, w.convertInt32(), h.convertInt32(), l.convertInt32());
	pushVal(L, mesh);
	return 1;
}

int LuaFuns::GenerateSphereMesh(lua_State* L)
{
	LuaVal radius = getVal(L, 1);
	LuaVal numSubdivisions = getVal(L, 2);
	if (!radius.isNumber() || !numSubdivisions.isNumber())
		return LUA_PARAM_ERROR(GenerateCubeMesh);
	MeshComponentPtr mesh = GeometryGenerator::instance()->createSphereMeshData(InputLayoutType::COMMON, radius.convertFloat(), numSubdivisions.convertInt32());
	pushVal(L, mesh);
	return 1;
}

int LuaFuns::AddComponent(lua_State* L)
{
	LuaVal obj = getVal(L, 1);
	LuaVal component = getVal(L, 2);
	if(!obj.isRefObj() || !component.isRefObj())
		return LUA_PARAM_ERROR(AddComponent);
	ObjectPtr objPtr = obj.convertRefPtr_dynamic<Object>();
	BaseComponentPtr compoPtr = component.convertRefPtr_dynamic<BaseComponent>();
	if (!objPtr.isValid() || !compoPtr.isValid())
		return LUA_PARAM_ERROR(AddComponent);
	objPtr->addComponent(compoPtr);
	return 0;
}

int LuaFuns::StartScene(lua_State* L)
{
	ScenePtr scene = getVal(L, 1).getField(LUA_CPP_REF_NAME).convertRefPtr_dynamic<Scene>();
	if(!scene.isValid())
		return LUA_PARAM_ERROR(StartScene);
	Engine::instance()->startScene(scene);
	return 0;
}

bool LuaFuns::registerFuncsToLua()
{
	Object::createLuaPrototype();
	REGIST_FUN(CreateScene);
//	REGIST_FUN(CreateObject);
	REGIST_FUN(CreateTexture);
	REGIST_FUN(CreateTransform);
	REGIST_FUN(CreateCamera);
	REGIST_FUN(CreateScriptComponent);
	REGIST_FUN(SetLocation);
	REGIST_FUN(SetRotation);
	REGIST_FUN(FaceToPoint);
	REGIST_FUN(AddObject);

	REGIST_FUN(GenerateCubeMesh);
	REGIST_FUN(GenerateSphereMesh);
	REGIST_FUN(AddComponent);
	REGIST_FUN(StartScene);
	return true;
}

bool LuaFuns::pushVal(lua_State * L, const LuaVal& val)
{
	return LuaManager::instance()->_pushVal(L, val);
}

LuaVal LuaFuns::getVal(lua_State * L, int index)
{
	return LuaManager::instance()->getVal(L, index);
}
