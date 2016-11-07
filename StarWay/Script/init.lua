function start()
	local scene = Scene:create()
	
	--add obj
	local obj = Object:create()
	
	local meshComponent = GenerateCubeMesh(2,2,2)
	obj:addComponent(meshComponent)
	
	local texture = TextureComponent:create("seafloor.dds","ShaderFirst_PS.cso")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(Vector3(1,0,1))
--	transform:setEulerRotation(Vector3(0,45,0))
	
	scene:addObject(obj)
	
	
	--add camera
	local cameraObj = Object:create()

	local cameraComponent = CameraComponent:create()
	cameraObj:addComponent(cameraComponent)
	
	local scriptComponent = ScriptComponent:create("CameraControl")
	cameraObj:addComponent(scriptComponent)

	local cametaTran = TransformComponent:create()
	cameraObj:addComponent(cametaTran)
	cametaTran:setLocation(Vector3(0, 2, -5))
	cametaTran:faceToPoint(Vector3(1, 0, 1))
	
	scene:addObject(cameraObj);
	
	--add light
	local directionLightObj = Object:create()
	local directionLightComponent = DirectionLightComponent:create(Vector4(0.2,0.2,0.2,1.0),Vector4(0.5,0.5,0.5,1.0),Vector4(0.5,0.5,0.5,1.0),Vector3(0.57735,-0.57735,0.57735))
	directionLightObj.addComponent(directionLightComponent)
	scene:addObject(directionLightObj);
	
	StartScene(scene)
end