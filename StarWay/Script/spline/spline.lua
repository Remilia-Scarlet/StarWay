Spline = {
	__index = self,
	create = function(controlPoint)
		local instance = {}
		setmetatable(instance,self)
		instance.controlPoint = controlPoint
		return instance
	end,
	get = function(t)
		if (t < 0.0 or t > 1.0) then
			error("wrong param")
		end
		
		local oneT = 1.0 / (self.controlPoint.len() - 1.0)
		local i = 0
		while ( i < (self.controlPoint.len() - 1) and t >= oneT) do
			t = t - oneT
			i = i + 1
		end
	
		t = t / oneT

		return (2.0 * math.pow(t, 3) - 3.0 * math.pow(t, 2) + 1.0) * self.controlPoint[i]
			+ (math.pow(t, 3) - 2.0 * math.pow(t, 2) + t) * self.getTangent(i)
			+ (-2.0 * math.pow(t, 3) + 3.0 * math.pow(t, 2)) * self.controlPoint[i + 1]
			+ (math.pow(t, 3) - math.pow(t, 2)) * self.getTangent(i + 1)
	end,
	getTangent = function(i)
		if ~(i >= 0 and i < self.controlPoint.len()) then
			error("wrong param")
		end
		if (self.controlPoint.len() == 1) then
			return self.controlPoint[0]
		end
		if (i == 0) then
			return (self.controlPoint[1] - self.controlPoint[0]) / (1.0 / (self.controlPoint.len() - 1.0))
		elseif (i == self.controlPoint.len() - 1) then
			return (self.controlPoint[self.controlPoint.len() - 1] - self.controlPoint[self.controlPoint.len() - 2]) / (1.0 / (self.controlPoint.len() - 1.0))
		else
			return (self.controlPoint[i + 1] - self.controlPoint[i - 1]) / (2.0 / (self.controlPoint.len() - 1.0))
		end
	end,
	controlPoint = {}
}