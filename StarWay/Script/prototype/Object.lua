--[[
Object = {}
Object.__index = Object
function Object:create()
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateObject()
	return obj
end 
function Object:addComponent(component)
	AddComponent(self.c_ref, component.c_ref)
end
]]