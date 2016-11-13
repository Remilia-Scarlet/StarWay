Nurbs = {
	
}
Nurbs.__index = Nurbs

function Nurbs:create(degree,controlPoints,knot,weight)
	local instance = {}
	setmetatable(instance,self)
	instance.n = degree
	instance.k = knot
	instance.P = controlPoints
	instance.w = weight
	return instance
end

function Nurbs:get(u)
	local a = Vector3(0,0,0)
	local b = 0
	Print(self)
	for i = 0,#(self.P)-1 do
		a = a + self.w[i]* self:N(i,self.n,u) * self.P[i] 
	end
	for i = 0,#(self.P)-1 do
		Print(self:N(i,self.n,u))
		b = b + self.w[i] * self:N(i,self.n,u)
	end

	Print(b)
	Print(a/b)
	return a / b
end

function Nurbs:N(i,n,u)
	print("N("..i..","..n..","..u..")")
	if(n == 0) then
		if(self.k[i] <= u and u < self.k[i + 1]) then
			return 1
		else
			return 0
		end
	end
	local result = self:f(i,n,u) * self:N(i,n-1,u) + self:g(i+1,n,u) * self:N(i+1,n-1,u)
	print("N("..i..","..n..","..u..")".." "..result)
	return result
end

function Nurbs:f(i,n,u)
	print("f "..i.." "..n.." "..u)
	if(self.k[i+n] == self.k[i])then
		return 0
	end
	local result = (u - self.k[i]) / (self.k[i+n] - self.k[i])
	print("result "..result)
	return result
end

function Nurbs:g(i,n,u)
	print("g "..i.." "..n.." "..u)
	if(self.k[i+n] == self.k[i])then
		return 0
	end
	local result = (self.k[i+n] - u) / (self.k[i+n] - self.k[i])
	print("result "..result)
	return result

end
