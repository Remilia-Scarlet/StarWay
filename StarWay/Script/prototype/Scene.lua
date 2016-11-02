Scene = {}
Scene.__index = Scene
function Scene:create()
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateScene()
	return obj
end 
function Scene:addObject(obj)
	AddObject(self.c_ref, obj.c_ref)
end