Nurbs = {
	
}
Nurbs.__index = Nurbs

function Nurbs:create()
	local instance = {}
	setmetatable(instance,self)
	return instance
end

function Nurbs:N(i,n,u)
	return f(i,n,u) * N(i,n-1,u) + g(i+1,n,u) * N(i+1,n-1,u)
end

function Nurbs:f(i,n,u)
	return (u - k[i]) / (k[i+n] - k[i])
end

