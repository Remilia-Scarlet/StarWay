SpineCamera = {
	init = function()
		local scene = GetCurrentScene()

		local ctlPoints = {Vector3(0,0,0),Vector3(2,5,0),Vector3(10,0,0),Vector3(10,-5,0),Vector3(14,-3,0),Vector3(10,2,0)}
		--[[for _,v in ipairs(ctlPoints) do
			local obj = CreateSimpleSphereOnPos(v)
			scene:addObject(obj)
		end]]
		
	--	SpineCamera.spline = Spline:create(ctlPoints)
		local ctr = {[0]=Vector3(0,0,0),Vector3(5,5,0),Vector3(10,0,0),Vector3(14,5,0),Vector3(17,0,0)}
		local nurbs = Nurbs:create(3,ctr,{[0]=1,1,1,2,3,4,5,5,5},{[0]=1,10,1,1,1})
	--	local ctr = {[0]=Vector3(4,-6,6),Vector3(-4,1,0),Vector3(1.5,5,-6),Vector3(0,2,-2),Vector3(1.5,5,-6),Vector3(4,1,0),Vector3(4,-6,6)}
	--	local nurbs = Nurbs:create(3,ctr,{[0]=1,2,3,4,5,5,6,7,8,9,9},{[0]=1,1,1,1,1,1,1})
		
	--[[	local ctr = {[0]=Vector3(6.0,  0.0,  6.0),
   Vector3(-5.5,  0.5,  5.5),
   Vector3(-5.0,  1.0, -5.0),
   Vector3( 4.5,  1.5, -4.5),
   Vector3( 4.0,  2.0,  4.0),
   Vector3(-3.5,  2.5,  3.5),
   Vector3(-3.0,  3.0, -3.0),
   Vector3( 2.5,  3.5, -2.5),
   Vector3( 2.0,  4.0,  2.0),
   Vector3(-1.5,  4.5,  1.5),
   Vector3(-1.0,  5.0, -1.0),
   Vector3( 0.5,  5.5, -0.5),
   Vector3( 0.0,  6.0,  0.0)}
		local nurbs = Nurbs:create(4,ctr,{[0]=0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10},{[0]=1,1,1,1,1,1,1,1,1,1,1,1,1})
		]]
		
		for _,v in ipairs(ctr) do
			local obj = CreateSimpleSphereOnPos(v)
			scene:addObject(obj)
		end
		
		local vertex = {}
		for t = 1.0,8.0,1 do
		--	local pos = SpineCamera.spline:get(t)
			local pos = nurbs:get(t)
			table.insert(vertex,{pos,Vector3(0,0,0),{x=0,y=0}} )
		end
		Print(vertex)

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
	--[[
		local obj = SpineCamera.obj
		SpineCamera.handleKeyChangeCamera(dt)
		local pos = SpineCamera.spline:get(SpineCamera.t)
		SpineCamera.t = SpineCamera.t + 1/60/5
		if(SpineCamera.t > 1)then
			SpineCamera.t = 0
		end
		local transformCom = obj:getComponent("TransformComponent")
		local offset = Vector3(0,0,-3)
		transformCom:setLocation(pos + offset)		]]
	end,
	freeCamera = {},
	spline = nil,
	t = 0.0
}

SpineCamera.handleKeyChangeCamera = function(dt)
	local obj = SpineCamera.obj
	local btnR3Status = InputManager:getVitualBtnStatus(InputManager.BTN_R3)
	local btnPauseStatus = InputManager:getVitualBtnStatus(InputManager.BTN_Pause)
	if(btnR3Status.isDown == true and btnR3Status.isChangedInThisFrame == true) then
		if(obj:getEnable() == true) then
			obj:setEnable(false)
			SpineCamera.freeCamera:setEnable(true)
		else
			obj:setEnable(true)
			SpineCamera.freeCamera:setEnable(false)
		end
	end
	if(btnPauseStatus.isDown == true and btnPauseStatus.isChangedInThisFrame == true) then
		obj:setEnable(false)
		SpineCamera.freeCamera:setEnable(true)
	end	
end