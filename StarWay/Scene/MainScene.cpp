#include "StarWay/precomp.h"
#include "MainScene.h"
#include "TinyEngine/Components/MeshComponent.h"
#include "StarWay/graph/StarVertex.h"
#include "TinyEngine/Components/TextureComponent.h"
#include "TinyEngine/Components/TransformComponent.h"
#include "TinyEngine/Components/CameraComponent.h"
#include "Graphic/Manager/InputLayoutDefine.h"
#include "Math/vector/Vector.h"
#include "Math/matrix/Matrix.h"
#include "Math/quaternion/Quaternion.h"
#include "TinyEngine/Other/Timer.h"

MainScenePtr MainScene::create()
{
	MainScene* ret = new MainScene();
	if (!ret || !ret->init())
	{
		TINY_SAFE_DELETE(ret);
	}
	return MainScenePtr(ret);
}

bool MainScene::start()
{
	do 
	{
		TINY_BREAK_IF(!Scene::start());

		return true;
	} while (0);
	return false;
}

bool MainScene::init()
{
	do 
	{
		TINY_BREAK_IF(!Scene::init());

		ObjectPtr obj = Object::create();

		std::vector<StarVertex> vertexBuffer =
		{
			StarVertex({-1.0f, 1.0f, -1.0f},{0.0f, 0.0f}),
			StarVertex({1.0f, 1.0f, -1.0f},{1.0f, 0.0f}),
			StarVertex({1.0f, 1.0f, 1.0f},{1.0f, 1.0f}),
			StarVertex({-1.0f, 1.0f, 1.0f},{0.0f, 1.0f}),

			StarVertex({-1.0f, -1.0f, -1.0f},{0.0f, 0.0f}),
			StarVertex({1.0f, -1.0f, -1.0f},{1.0f, 0.0f}),
			StarVertex({1.0f, -1.0f, 1.0f},{1.0f, 1.0f}),
			StarVertex({-1.0f, -1.0f, 1.0f},{0.0f, 1.0f}),

			StarVertex({-1.0f, -1.0f, 1.0f},{0.0f, 0.0f}),
			StarVertex({-1.0f, -1.0f, -1.0f},{1.0f, 0.0f}),
			StarVertex({-1.0f, 1.0f, -1.0f},{1.0f, 1.0f}),
			StarVertex({-1.0f, 1.0f, 1.0f},{0.0f, 1.0f}),

			StarVertex({1.0f, -1.0f, 1.0f},{0.0f, 0.0f}),
			StarVertex({1.0f, -1.0f, -1.0f},{1.0f, 0.0f}),
			StarVertex({1.0f, 1.0f, -1.0f},{1.0f, 1.0f}),
			StarVertex({1.0f, 1.0f, 1.0f},{0.0f, 1.0f}),

			StarVertex({-1.0f, -1.0f, -1.0f},{0.0f, 0.0f}),
			StarVertex({1.0f, -1.0f, -1.0f},{1.0f, 0.0f}),
			StarVertex({1.0f, 1.0f, -1.0f},{1.0f, 1.0f}),
			StarVertex({-1.0f, 1.0f, -1.0f},{0.0f, 1.0f}),

			StarVertex({-1.0f, -1.0f, 1.0f},{0.0f, 0.0f}),
			StarVertex({1.0f, -1.0f, 1.0f},{1.0f, 0.0f}),
			StarVertex({1.0f, 1.0f, 1.0f},{1.0f, 1.0f}),
			StarVertex({-1.0f, 1.0f, 1.0f},{0.0f, 1.0f}),
		};
		std::vector<short> indices =
		{
			3,1,0, //top
			2,1,3,

			6,4,5, //bottom
			7,4,6,

			11,9,8,
			10,9,11,

			14,12,13,
			15,12,14,

			19,17,16,
			18,17,19,

			22,20,21,
			23,20,22
		};
		MeshComponentPtr mesh = MeshComponent::create(InputLayoutType::COMMON, vertexBuffer, indices,"ShaderFirst_VS.cso");
		obj->addComponent(mesh);

		TextureComponentPtr texture = TextureComponent::create("seafloor.dds", "ShaderFirst_PS.cso");
		obj->addComponent(texture);
		
		TransformComponentPtr transform = TransformComponent::create();
		obj->addComponent(transform);
		Timer(0, -1, [transform](Timer* timer) {
			return;
			transform->setLocation( (float)sin(timer->getPassedTime()) * 2.f , transform->getLocationY(), transform->getLocationZ());
		//	transform->setRotation(0, (float)cos(timer->getPassedTime()) * 360 ,0);
			Quaternion rotation(Vector3{ 0,0,3 }, Vector3{ 0,0,3 } - transform->getLocation());
			Vector3 eular = rotation.toEularAngle();
			eular.Z() = (float)sin(timer->getPassedTime()) * 360;
			transform->setRotation(Quaternion(eular));
		}).start();
		transform->setScale(0.6f, 0.6f, 0.6f);
		transform->setLocation(Vector3(0, 0, 4));

		addObject(obj);


		//add camera
		ObjectPtr cameraObj = Object::create();

		CameraComponentPtr camera = CameraComponent::create();
		cameraObj->addComponent(camera);

		TransformComponentPtr cametaTran = TransformComponent::create();
		cameraObj->addComponent(cametaTran);
	//	cametaTran->setLocation(0, 2, -5);
	//	cametaTran->setLocation(0, 0, 0);
		cametaTran->faceToPoint(Vector3( 0,0,0 ));

		addObject(cameraObj);

		return true;
	} while (0);
	return false;
}
