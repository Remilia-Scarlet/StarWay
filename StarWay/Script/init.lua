function start()
	local scene = Scene:create()
	
	CreateGround(scene)
	CreateObj(scene)
	CreateApple(scene)
	CreateDangoCube(scene)
	local freeCamera = CreateFreeCamera(scene)
	--CreateSplineCamera(scene,freeCamera)
	CreateLight(scene)
	
	StartScene(scene)
end

function CreateGround(scene)
	local obj = Object:create()
	
	local meshComponent = GenerateCubeMesh(30,1,30)
	obj:addComponent(meshComponent)
	
	local texture = TextureComponent:create()
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(Vector3(0,-4,0))
	
	scene:addObject(obj)

end

function CreateObj(scene)
	--add obj
	local obj = Object:create()
	
	local meshComponent = GenerateCubeMesh(2,2,2)
	obj:addComponent(meshComponent)
	
	local texture = TextureComponent:create("seafloor.dds")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(Vector3(-3,0,0))
--	transform:setEulerRotation(Vector3(0,45,0))
	scene:addObject(obj)

	local sphereObj = Object:create()
	local trans = TransformComponent:create()
	sphereObj:addComponent(trans)
	trans:setLocation(Vector3(4, 0, -3))
	local meshConponent = GenerateSphereMesh(1.5,4)
	sphereObj:addComponent(meshConponent)
	local texture = TextureComponent:create()
	sphereObj:addComponent(texture)
	scene:addObject(sphereObj)
	

end

function CreateApple(scene)
	local objs = LoadObj("DangoApple.obj")
	for _,obj in pairs(objs) do
		local transform = TransformComponent:create()
		obj:addComponent(transform)
		scene:addObject(obj)
	end
end

function CreateDangoCube(scene)
	local objs = LoadObj("DangoCube.obj")
	for _,obj in pairs(objs) do
		local transform = TransformComponent:create()
		obj:addComponent(transform)
		transform:setLocation(Vector3(2,0,0))
		scene:addObject(obj)
	end
end

function CreateFreeCamera(scene)
	--add free camera
	local freeCamera = Object:create()

	local cameraComponent = CameraComponent:create()
	freeCamera:addComponent(cameraComponent)
	
	local scriptComponent = ScriptComponent:create("CameraControl")
	freeCamera:addComponent(scriptComponent)

	local cametaTran = TransformComponent:create()
	freeCamera:addComponent(cametaTran)
	cametaTran:setLocation(Vector3(0, 2, -5))
--	cametaTran:faceToPoint(Vector3(1, 0, 1))
	
	scene:addObject(freeCamera);
	return freeCamera
end

function CreateSplineCamera(scene,freeCamera)
	--add spine camera
	local spineCamera = Object:create()
	spineCamera:setEnable(false)
	
	local spineCameraComponent = CameraComponent:create()
	spineCamera:addComponent(spineCameraComponent)
	
	local spineScriptComponent = ScriptComponent:create("SplineCamera")
	spineCamera:addComponent(spineScriptComponent)
	SplineCamera.freeCamera = freeCamera

	local spineCametaTran = TransformComponent:create()
	spineCamera:addComponent(spineCametaTran)
	spineCametaTran:setLocation(Vector3(0, 2, -5))
	spineCametaTran:faceToPoint(Vector3(1, 0, 1))
	
	scene:addObject(spineCamera);
end


function CreateLight(scene)
	--direction light
	local directionLightObj = Object:create()
	local directionLightComponent = DirectionLightComponent:create(Vector4(0.6,0.6,0.6,1),Vector4(1,1,1,1.0),Vector4(0.0,0.0,0.0,1.0),Vector3(-0.707,-0.707,1))
	directionLightObj:addComponent(directionLightComponent)
	scene:addObject(directionLightObj);
	
	--point light
	local pointLightObj = Object:create()
	local pointLightComponent = PointLightComponet:create(Vector4(0.0,0.0,0.0,1.0),Vector4(1,1,1,1.0),Vector4(0.0,0.0,0.0,1.0),20.0,Vector3(0.0,0.1,0.05))
	pointLightObj:addComponent(pointLightComponent)
	local trans = TransformComponent:create()
	pointLightObj:addComponent(trans)
	trans:setLocation(Vector3(0, 0, 0))
	local meshConponent = GenerateSphereMesh(0.5,3)
	pointLightObj:addComponent(meshConponent)
	local texture = TextureComponent:create("seafloor.dds","point_light_PS.cso")
	pointLightObj:addComponent(texture)
	local pointLightScript = ScriptComponent:create("PointLightMovement")
	pointLightObj:addComponent(pointLightScript)
	
	scene:addObject(pointLightObj)
	
end