function start()
	local scene = Scene:create()
	
	CreateGround(scene)
	CreateObj(scene)
	local freeCamera = CreateFreeCamera(scene)
	CreateSplineCamera(scene,freeCamera)
	CreateSpline(scene)
	CreateLight(scene)
	
	StartScene(scene)
end

function CreateGround(scene)
	local size = 2
	for i = -10 , 10 do
		for j = -10 , 10 do
			local obj = Object:create()
			
			local meshComponent = GenerateCubeMesh(size,0.0,size)
			obj:addComponent(meshComponent)
			
			local texture = TextureComponent:create("","")
			obj:addComponent(texture)
			
			local transform = TransformComponent:create()
			obj:addComponent(transform)
			transform:setLocation(Vector3(i*size,-2,j*size))
			
			scene:addObject(obj)
		end
	end 
end

function CreateObj(scene)
	--add obj
	local obj = Object:create()
	
	local meshComponent = GenerateCubeMesh(2,2,2)
	obj:addComponent(meshComponent)
	
	local texture = TextureComponent:create("seafloor.dds","ShaderFirst_PS.cso")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(Vector3(1,-1,1))
--	transform:setEulerRotation(Vector3(0,45,0))
	
	scene:addObject(obj)
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

function CreateSpline(scene)
--	local controlPoints = {Vector3(0,0,0),Vector3(1,1,0),Vector3(2,0,0),Vector3(1,-1,0) }
--	Spline:create(controlPoints)
	
end

function CreateLight(scene)
	--add light
	local directionLightObj = Object:create()
	local directionLightComponent = DirectionLightComponent:create(Vector4(0.2,0.2,0.2,1.0),Vector4(0.5,0.5,0.5,1.0),Vector4(0.5,0.5,0.5,1.0),Vector3(0.57735,-0.57735,0.57735))
	directionLightObj:addComponent(directionLightComponent)
	scene:addObject(directionLightObj);
end