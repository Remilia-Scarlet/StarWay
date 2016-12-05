PointLightMovement = {
	ROTATE_RANDIUS = 5.0,
	ROTATE_SPEED = 3,
	originalPos = nil,
	detaTime = 0.0
}

function PointLightMovement:init()
	local obj = PointLightMovement.obj
	local transformCom = obj:getComponent("TransformComponent")
	if(transformCom == nil)then
		error("transform component nil")
	end
	PointLightMovement.originalPos = transformCom:getLocation()
end

function PointLightMovement:update(dt)
	local obj = PointLightMovement.obj
	local transformCom = obj:getComponent("TransformComponent")
	if(transformCom == nil)then
		error("transform component nil")
	end
	self.detaTime = self.detaTime + dt
	if self.detaTime > self.ROTATE_SPEED then
		self.detaTime = self.detaTime - self.ROTATE_SPEED
	end
	local x = self.originalPos.x
	local y = self.originalPos.z + self.ROTATE_RANDIUS
	local x0 = self.originalPos.x
	local y0 = self.originalPos.z
	local angle = self.detaTime / self.ROTATE_SPEED * math.pi * 2.0
	local cosTheta = math.cos(angle)
	local sinTheta = math.sin(angle)
	x = (x-x0) * cosTheta - (y-y0) * sinTheta + x0
	y = (x-x0) * sinTheta + (y-y0) * cosTheta + y0
	transformCom:setLocation(Vector3(x,self.originalPos.y,y))
end

