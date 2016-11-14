Spline = {
	controlPoint = {}
}
Spline.__index = Spline

function Spline:create(ctlPoint)
	local instance = {controlPoint = ctlPoint,tTab = {}}
	setmetatable(instance,self)
	instance:calcT()
	return instance
end

function Spline:get(t)
	local i = #self.tTab - 1
	for index = 1,#self.tTab do
		if(self.tTab[index] > t) then
			i = index - 1
			break
		end
	end
	
	t = t - self.tTab[i]
	
	t = t / (self.tTab[i + 1] - self.tTab[i])
	
	local pos = (2.0 * t^3 - 3.0 * t^2 + 1.0) * self.controlPoint[i]
		+ (t^3 - 2.0 * t^2 + t) * self:getTangent(i)
		+ (-2.0 * t^3 + 3.0 * t^2) * self.controlPoint[i+1]
		+ (t^3 - t^2) * self:getTangent(i+1)
		
	return pos
end

function Spline:getTangent(i)
	if not (i > 0 and i <= #self.controlPoint) then
		error("wrong param")
	end
	if (#self.controlPoint == 1) then
		return self.controlPoint[1]
	end
	if (i == 1) then
		local sum = self.tTab[i + 1] - self.tTab[i]
		return (self.controlPoint[i + 1] - self.controlPoint[i]) / (self.tTab[i + 1] - self.tTab[i]) * sum
	elseif (i == #self.controlPoint) then
		local sum = self.tTab[i] - self.tTab[i - 1]
		return (self.controlPoint[i] - self.controlPoint[i - 1]) / (self.tTab[i] - self.tTab[i - 1]) * sum
	else
		local sum = self.tTab[i + 1] - self.tTab[i - 1]
		return 0.5 * ( (self.controlPoint[i + 1] - self.controlPoint[i]) / (self.tTab[i + 1] - self.tTab[i]) * sum 
			+ (self.controlPoint[i] - self.controlPoint[i - 1]) / (self.tTab[i] - self.tTab[i - 1]) * sum  )
	end
end

function Spline:calcT(i)
	local lenTab = {0}
	for i = 2,#self.controlPoint do
		table.insert(lenTab,Vector3Len(self.controlPoint[i] - self.controlPoint[i - 1]))
	end
	local totolLen = 0.0
	for j = 1,#lenTab do
		totolLen = totolLen + lenTab[j]
	end
	local currentLen = 0
	for k = 1,#lenTab do
		currentLen = currentLen + lenTab[k]
		self.tTab[k] = currentLen / totolLen
	end
end