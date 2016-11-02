Mesh = {}
Mesh.__index = Mesh
function Mesh:create()
	local obj = {}
	setmetatable(obj,self)
	--obj.c_ref = 
	return obj
end 

function Mesh:generateCube(width,height,lenth)
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = GenerateCubeMesh(width,height,lenth)
	return obj
end 

function Mesh:generateSphere(radius, numSubdivisions)
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = GenerateSphereMesh(radius, numSubdivisions)
	return obj
end 