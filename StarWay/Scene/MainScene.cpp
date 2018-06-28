#include "precomp.h"
#include "MainScene.h"
#include "TinyEngine/Objects/Object.h"
#include "Graphic/GeometryGenerator.h"
#include "Components/TextureComponent.h"
#include "Components/TransformComponent.h"
#include "Graphic/WavefrontObjReader.h"
#include "Components/CameraComponent.h"
#include "Components/ScriptComponent.h"
#include "Scripts/CameraControl.h"
#include "Components/DirectionLightComponent.h"
#include "Components/PointLightComponet.h"
#include "Scripts/PointLightMovement.h"

MainScenePtr MainScene::create()
{
	MainScene* scene = new MainScene();
	if(scene && scene->init())
	{
		return MainScenePtr(scene);
	}
	return nullptr;
}

bool MainScene::init()
{
	if(!Scene::init())
		return false;
	createGround();
	createObj();
	createApple();
	createDangoCube();
	createFreeCamera();
	createLight();
	return true;
}

void MainScene::createGround()
{
	ObjectPtr obj = Object::create();

	MeshComponentPtr meshComponent = GeometryGenerator::instance()->createCubeMesh(30, 1, 30);
	obj->addComponent(meshComponent);

	TextureComponentPtr texture = TextureComponent::create();
	obj->addComponent(texture);

	TransformComponentPtr transform = TransformComponent::create();
	obj->addComponent(transform);
	transform->setLocation(Vector3(0, -4, 0));

	this->addObject(obj);
}

void MainScene::createObj()
{
	ObjectPtr obj = Object::create();

	MeshComponentPtr meshComponent = GeometryGenerator::instance()->createCubeMesh(2, 2, 2);
	obj->addComponent(meshComponent);

	TextureComponentPtr texture = TextureComponent::create("game:seafloor.dds", "game:shaders/pass_through_no_tex.ps.cso");
	obj->addComponent(texture);

	TransformComponentPtr transform = TransformComponent::create();
	obj->addComponent(transform);
	transform->setLocation(Vector3(-3, 0, 0));
	//transform -> setEulerRotation(Vector3(0, 45, 0));
	this->addObject(obj);

	ObjectPtr sphereObj = Object::create();
	TransformComponentPtr trans = TransformComponent::create();
	sphereObj->addComponent(trans);
	trans->setLocation(Vector3(3, 0, -3));

	MeshComponentPtr meshConponent = GeometryGenerator::instance()->createSphereMeshData(1.5, 4);
	sphereObj->addComponent(meshConponent);

	TextureComponentPtr texture2 = TextureComponent::create();
	sphereObj->addComponent(texture2);

	this->addObject(sphereObj);
}

void MainScene::createApple()
{
	WavefrontObjReader reader("game:DangoApple.obj");
	std::vector<ObjectPtr> outObj;
	reader.readObjFile(outObj);
	for(ObjectPtr& obj : outObj)
	{
		TransformComponentPtr transform = TransformComponent::create();
		obj->addComponent(transform);
		addObject(obj);
	}
}

void MainScene::createDangoCube()
{
	WavefrontObjReader reader("game:DangoCube.obj");
	std::vector<ObjectPtr> outObj;
	reader.readObjFile(outObj);
	for (ObjectPtr& obj : outObj)
	{
		TransformComponentPtr transform = TransformComponent::create();
		transform->setLocation(6, 0, 0);
		obj->addComponent(transform);
		addObject(obj);
	}
}

void MainScene::createFreeCamera()
{
	ObjectPtr freeCamera = Object::create();

	CameraComponentPtr cameraComponent = CameraComponent::create();
	freeCamera->addComponent(cameraComponent);

	ScriptComponentPtr scriptComponent = ScriptComponent::create(std::make_shared<CameraControl>());
	freeCamera->addComponent(scriptComponent);

	TransformComponentPtr cametaTran = TransformComponent::create();
	freeCamera->addComponent(cametaTran);
	cametaTran->setLocation(Vector3(0, 2, -5));

	addObject(freeCamera);
}

void MainScene::createLight()
{
	//direction light
	ObjectPtr directionLightObj = Object::create();
	DirectionLightComponentPtr directionLightComponent = DirectionLightComponent::create(
		Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1));
	directionLightObj->addComponent(directionLightComponent);
	TransformComponentPtr directionLightTrans = TransformComponent::create();
	directionLightTrans->setEulerRotation(Vector3(45, 45, 0));
	directionLightObj->addComponent(directionLightTrans);
	addObject(directionLightObj);
	
	//point light
	ObjectPtr pointLightObj = Object::create();
	PointLightComponetPtr pointLightComponent = PointLightComponet::create(
		Vector4(0, 0, 0, 0), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), 20.0, Vector3(0.0f, 0.1f, 0.05f));
	pointLightObj->addComponent(pointLightComponent);
	TransformComponentPtr trans = TransformComponent::create();
	pointLightObj->addComponent(trans);
	trans->setLocation(Vector3(0, 0, 0));
	MeshComponentPtr meshConponent = GeometryGenerator::instance()->createSphereMeshData(0.5, 3);
	pointLightObj->addComponent(meshConponent);
	TextureComponentPtr texture = TextureComponent::create("game:seafloor.dds", "game:point_light_PS.cso");
	pointLightObj->addComponent(texture);
	ScriptComponentPtr pointLightScript = ScriptComponent::create(std::make_shared<PointLightMovement>());
	pointLightObj->addComponent(pointLightScript);
	
	addObject(pointLightObj);
}
