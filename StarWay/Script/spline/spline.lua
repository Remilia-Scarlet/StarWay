Spline = {
	controlPoint = {}
}
Spline.__index = Spline

function Spline:create(ctlPoint)
	local instance = {controlPoint = ctlPoint}
	setmetatable(instance,self)
	return instance
end

function Spline:get(t)
	if (t < 0.0 or t > 1.0) then
		error("wrong param")
	end
	
	local oneT = 1.0 / (#self.controlPoint - 1.0)
	local i = 1
	while ( i < (#self.controlPoint) and t >= oneT) do
		t = t - oneT
		i = i + 1
	end

	t = t / oneT

	return (2.0 * t^3 - 3.0 * t^2 + 1.0) * self.controlPoint[i]
		+ (t^3 - 2.0 * t^2 + t) * self:getTangent(i)
		+ (-2.0 * t^3 + 3.0 * t^2) * self.controlPoint[i + 1]
		+ (t^3 - t^2) * self:getTangent(i + 1)
end
function Spline:getTangent(i)
	if not (i > 0 and i <= #self.controlPoint) then
		error("wrong param")
	end
	if (#self.controlPoint == 1) then
		return self.controlPoint[1]
	end
	if (i == 1) then
		return (self.controlPoint[2] - self.controlPoint[1]) / (1.0 / (#self.controlPoint - 1.0))
	elseif (i == #self.controlPoint) then
		return (self.controlPoint[#self.controlPoint] - self.controlPoint[#self.controlPoint - 1]) / (1.0 / (#self.controlPoint - 1.0))
	else
		return (self.controlPoint[i + 1] - self.controlPoint[i - 1]) / (2.0 / (#self.controlPoint - 1.0))
	end
end
