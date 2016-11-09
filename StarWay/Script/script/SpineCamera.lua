SpineCamera = {
	update = function(dt)
		local obj = SpineCamera.obj
		SpineCamera.handleKeyChangeCamera(dt)
		if(SpineCamera.spline == nil)then
			local ctlPoints = {Vector3(0,0,0),Vector3(5,5,0),Vector3(10,0,0),Vector3(5,-5,0)}
			SpineCamera.spline = Spline:create(ctlPoints)
		end
		local pos = SpineCamera.spline:get(SpineCamera.t)
		SpineCamera.t = SpineCamera.t + 0.01
		if(SpineCamera.t > 1)then
			SpineCamera.t = 0
		end
		local transformCom = obj:getComponent("TransformComponent")
		transformCom:setLocation(pos)
		
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