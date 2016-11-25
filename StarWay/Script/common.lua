function CppCallLua(tab,...)
	local callFun = _G
	for i,f in ipairs(tab) do
		if callFun ~= nil then
			callFun = callFun[f]
		end
	end
	if(callFun ~= _G and callFun ~= nil) then
		callFun(...)
	end
end

function Vector2(x,y)
	return LuaVector:create(2,x,y)
end

function Vector3(x,y,z)
	return LuaVector:create(3,x,y,z)
end

function Vector4(x,y,z,w)
	return LuaVector:create(4,x,y,z,w)
end
--[[
function Rect(x,y,w,h)
	return {x=x,y=y,w=w,h=h}
end]]

function CreateSimpleCubeOnPos(pos)
	local obj = Object:create()
	
	local meshConponent = GenerateCubeMesh(0.5,0.5,0.5)
	obj:addComponent(meshConponent)
	
	local texture = TextureComponent:create("","")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(pos)
	return obj
end
function CreateSimpleSphereOnPos(pos)
	local obj = Object:create()
	
	local meshConponent = GenerateSphereMesh(0.1,1)
	obj:addComponent(meshConponent)
	
	local texture = TextureComponent:create("","")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(pos)
	return obj
end