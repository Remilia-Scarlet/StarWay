

--function Object:addComponent(component)
--	addComponent(self,component)
--end

function start()
	local scene = Scene:create()
	
	--add obj
	local obj = Object:create()

	local meshComponent = Mesh:generateCube(2,2,2)
	obj:addComponent(meshComponent)
	
	local texture = Texture:create("seafloor.dds","ShaderFirst_PS.cso")
	obj:addComponent(texture)
	
	local transform = Transform:create()
	obj:addComponent(transform)
	transform:setLocation(1,0,1)
	transform:setRotation(0,45,0)
	
	local scriptComponent = ScriptComponent:create("ObjUpdate")
	obj:addComponent(scriptComponent)
	
	scene:addObject(obj)
	
	
	--add camera
	local cameraObj = Object:create()

	local cameraComponent = Camera:create()
	cameraObj:addComponent(cameraComponent)

	local cametaTran = Transform:create()
	cameraObj:addComponent(cametaTran)
	cametaTran:setLocation(0, 2, -5)
	cametaTran:faceToPoint(0, 0, 0)
	
	scene:addObject(cameraObj);
	
	StartScene(scene)
end