Nurbs = {
	
}
Nurbs.__index = Nurbs

function Nurbs:create()
	local instance = {}
	setmetatable(instance,self)
	return instance
end

function Nurbs:get(u)
	local a = {}
	local b = 0
	for i = 1,#self.P do
		a = a + self.w[i]* self:N[i,k,u] * self.P[i] 
	end
	for i = 1,#self.P do
		b = b + self.w[i] * self:N[i,k,u]
	end
	return a / b
end

function Nurbs:N(i,n,u)
	if(n == 0) then
		if(k[i] <= u and u < k[i + i]) then
			return 1
		else
			return 0
		end
	end
	return self:f(i,n,u) * self:N(i,n-1,u) + self:g(i+1,n,u) * self:N(i+1,n-1,u)
end

function Nurbs:f(i,n,u)
	return (u - self:k[i]) / (self:k[i+n] - self:k[i])
end

function Nurbs:g(i,n,u)
	return (self:k[i+n] - u) / (self:k[i+n] - self:k[i])
end
