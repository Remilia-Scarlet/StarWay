SpineCamera = {
	init = function()
		local scene = GetCurrentScene()

		local ctlPoints = {Vector3(0,0,0),Vector3(2,5,0),Vector3(10,0,0),Vector3(10,-5,0),Vector3(14,-3,0),Vector3(10,2,0)}
		for _,v in ipairs(ctlPoints) do
			local obj = CreateSimpleSphereOnPos(v)
			scene:addObject(obj)
		end
		
		SpineCamera.spline = Spline:create(ctlPoints)
		
		local vertex = {}
		for t = 0.0,1.0,0.01 do
			local pos = SpineCamera.spline:get(t)
			table.insert(vertex,{pos,Vector3(0,0,0),{x=0,y=0}} )
		end

		local obj = Object:create()
		
		local meshConponent = MeshComponent:create(vertex,"ShaderFirst_VS.cso")
		meshConponent:setPrimitiveTopology(MeshComponent.LINE_STRIP)
		obj:addComponent(meshConponent)
		
		local texture = TextureComponent:create("","")
		obj:addComponent(texture)
		
		local transform = TransformComponent:create()
		obj:addComponent(transform)
		transform:setLocation(Vector3(0,0,0))
		
		scene:addObject(obj)
	end,
	update = function(dt)
		local obj = SpineCamera.obj
		SpineCamera.handleKeyChangeCamera(dt)
		local pos = SpineCamera.spline:get(SpineCamera.t)
		SpineCamera.t = SpineCamera.t + 1/60/5
		if(SpineCamera.t > 1)then
			SpineCamera.t = 0
		end
		local transformCom = obj:getComponent("TransformComponent")
		local up = Vector3(0,0,-2)
		transformCom:setLocation(pos + up)		
	end,
	freeCamera = {},
	spline = nil,
	t = 0.0
}

SpineCamera.handleKeyChangeCamera = function(dt)
	local obj = SpineCamera.obj
	local btnR3Status = InputManager:getVitualBtnStatus(InputManager.BTN_R3)
	if(btnR3Status.isDown == true and btnR3Status.isChangedInThisFrame == true) then
		if(obj:getEnable() == true) then
			obj:setEnable(false)
			SpineCamera.freeCamera:setEnable(true)
		else
			obj:setEnable(true)
			SpineCamera.freeCamera:setEnable(false)
		end
	end
end