Bezier = {
}
Bezier.__index = Bezier

function Bezier:create(degree,controlPoints)
	local instance = {}
	setmetatable(instance,self)
	instance.degree = degree
	instance.controlPoints = controlPoints
	return instance
end

function Bezier:get(t)
	if(self.degree == 1)then
		return (1-t) * self.controlPoints[1] + t * self.controlPoints[2]
	elseif(self.degree == 2)then
		return (1-t)^2 * self.controlPoints[1] + 2 * t * (1-t) * self.controlPoints[2] + t^2 * self.controlPoints[3]
	elseif(self.degree == 3)then
		return (1-t)^3 * self.controlPoints[1] + 3 * t * (1-t)^2 * self.controlPoints[2] + 3 * t^2 * (1-t) * self.controlPoints[3] + t^3 * self.controlPoints[4]
	end
end