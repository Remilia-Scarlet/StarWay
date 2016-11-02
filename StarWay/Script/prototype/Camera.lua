Camera = {}
Camera.__index = Camera
function Camera:create()
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateCamera()
	return obj
end 

function Camera:faceToPoint(x,y,z)
	FaceToPoint(self.c_ref,x,y,z)
end