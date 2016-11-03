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
	transform:setLocation(1,0,1)
	transform:setRotation(0,45,0)
	
	local scriptComponent = ScriptComponent:create("ObjUpdate")
	obj:addComponent(scriptComponent)
	
	scene:addObject(obj)
	
	
	--add camera
	local cameraObj = Object:create()

	local cameraComponent = CameraComponent:create()
	cameraObj:addComponent(cameraComponent)

	local cametaTran = TransformComponent:create()
	cameraObj:addComponent(cametaTran)
	cametaTran:setLocation(0, 2, -5)
	cametaTran:faceToPoint(0, 0, 0)
	
	scene:addObject(cameraObj);
	
	StartScene(scene)
end