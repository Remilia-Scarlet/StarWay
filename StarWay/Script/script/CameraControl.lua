CameraControl = {
	update = function(dt)
		CameraControl.handleMouse(dt)
		CameraControl.handleKey(dt)
	end,
	MOVE_SPEED = 0.1,
	TURN_SPEED = 0.1,
	mouseInfo = {lastLIsDown = false, lastMousePos={x=0,y=0}}
}

CameraControl.handleMouse = function(dt)
	local obj = CameraControl.obj
	local mouseInfo = CameraControl.mouseInfo
	local transformCom = obj:getComponent("TransformComponent")
	if(transformCom == nil)then
		error("transform component nil")
	end

	local mouseRStatus = InputManager:getVirtualMouseStatus(InputManager.MOUSE_R)
	local mouseLStatus = InputManager:getVirtualMouseStatus(InputManager.MOUSE_L)
	if(mouseLStatus.isDown == true)then
		if(mouseInfo.lastLIsDown)then
			local currentMousePos = InputManager:getVirtualMousePos()
			local detaX = currentMousePos.x - mouseInfo.lastMousePos.x
			local detaY = currentMousePos.y - mouseInfo.lastMousePos.y
			local euler = transformCom:getEulerRotation()
			euler.y = euler.y + detaX * CameraControl.TURN_SPEED
			euler.x = euler.x + detaY * CameraControl.TURN_SPEED
			transformCom:setEulerRotation(euler)
			mouseInfo.lastMousePos = currentMousePos
		else
			mouseInfo.lastLIsDown = true
			mouseInfo.lastMousePos = InputManager:getVirtualMousePos()
		end
	else
		mouseInfo.lastLIsDown = false
	end
end

CameraControl.handleKey = function(dt)
	local obj = CameraControl.obj
	local transformCom = obj:getComponent("TransformComponent")
	if(transformCom == nil)then
		error("transform component nil")
	end
	
	local btnUpStatus = InputManager:getVitualBtnStatus(InputManager.BTN_UP)
	local btnDownStatus = InputManager:getVitualBtnStatus(InputManager.BTN_DOWN)
	local btnLeftStatus = InputManager:getVitualBtnStatus(InputManager.BTN_LEFT)
	local btnRightStatus = InputManager:getVitualBtnStatus(InputManager.BTN_RIGHT)
	local btnL1Status = InputManager:getVitualBtnStatus(InputManager.BTN_L1)
	local btnR1Status = InputManager:getVitualBtnStatus(InputManager.BTN_R1)
	local btnPauseStatus = InputManager:getVitualBtnStatus(InputManager.BTN_Pause)
	if(btnPauseStatus.isDown ~= true and btnUpStatus.isDown ~= true and btnDownStatus.isDown ~= true and btnRightStatus.isDown ~= true and btnLeftStatus.isDown ~= true and btnL1Status.isDown ~=true and btnR1Status.isDown ~= true) then
		return
	end
	
	local location = transformCom:getLocation()
	local frontDirection = transformCom:getFrontDirection()
	local rightDirection = transformCom:getRightDirection()
	local speed = CameraControl.MOVE_SPEED
--	Print("front",frontDirection)
	if(btnUpStatus.isDown)then
		location.x = location.x + frontDirection.x * speed
		location.y = location.y + frontDirection.y * speed
		location.z = location.z + frontDirection.z * speed
	elseif (btnDownStatus.isDown) then 
		location.x = location.x - frontDirection.x * speed
		location.y = location.y - frontDirection.y * speed
		location.z = location.z - frontDirection.z * speed
	end
	if(btnRightStatus.isDown) then
		location.x = location.x + rightDirection.x * speed
		location.y = location.y + rightDirection.y * speed
		location.z = location.z + rightDirection.z * speed
	elseif(btnLeftStatus.isDown) then
		location.x = location.x - rightDirection.x * speed
		location.y = location.y - rightDirection.y * speed
		location.z = location.z - rightDirection.z * speed
	end
	if(btnL1Status.isDown)then
		local parentUp = transformCom:parentVectorToLocal(Vector3(0,1,0))
		--Print(parentUp)
		location.x = location.x + parentUp.x * speed
		location.y = location.y + parentUp.y * speed
		location.z = location.z + parentUp.z * speed
	elseif (btnR1Status.isDown) then 
		local parentUp = transformCom:parentVectorToLocal(Vector3(0,1,0))
		--Print(parentUp)
		location.x = location.x - parentUp.x * speed
		location.y = location.y - parentUp.y * speed
		location.z = location.z - parentUp.z * speed
	end
	if(btnPauseStatus.isDown == true) then
		transformCom:setLocation(Vector3(0, 2, -5))
	else
		transformCom:setLocation(location)
	end
end