Transform = {}
Transform.__index = Transform
function Transform:create()
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateTransform()
	return obj
end

function Transform:setLocation(x,y,z)
	SetLocation(self.c_ref,x,y,z)
end

function Transform:setRotation(x,y,z)
	SetRotation(self.c_ref,x,y,z)
end

function Transform:faceToPoint(x,y,z)
	FaceToPoint(self.c_ref,x,y,z)
end