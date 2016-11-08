SpineCamera = {
	update = function(dt)
		SpineCamera.handleKeyChangeCamera(dt)
		
	end,
	freeCamera = {}
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