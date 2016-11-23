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

Vector3MetaTable = {}
Vector3MetaTable.__add = function(a,b)
	local vec = {x = a.x + b.x, y = a.y + b.y, z = a.z + b.z}
	setmetatable(vec,Vector3MetaTable)
	return vec
end
Vector3MetaTable.__sub = function(a,b)
	local vec = {x = a.x - b.x, y = a.y - b.y, z = a.z - b.z}
	setmetatable(vec,Vector3MetaTable)
	return vec
end
Vector3MetaTable.__mul = function(a,b)
	if type(a) == "number" and type(b) == "table" then
		local vec = {x = b.x * a , y = b.y * a, z = b.z * a}
		setmetatable(vec,Vector3MetaTable)
		return vec
	elseif type(a) == "table" and type(b) == "number" then
		local vec = {x = a.x * b , y = a.y * b, z = a.z * b}
		setmetatable(vec,Vector3MetaTable)
		return vec
	end
end
Vector3MetaTable.__div = function(a,b)
	if type(a) == "table" and type(b) == "number" then
		local vec = {x = a.x / b , y = a.y / b, z = a.z / b}
		setmetatable(vec,Vector3MetaTable)
		return vec
	end
end

function Vector3Len(vec3)
	return (vec3.x ^ 2 + vec3.y ^ 2 + vec3.z ^ 2) ^ 0.5
end

function Vector3(x,y,z)
	return LuaVector:create(3)
	--[[
	local v = {x=x,y=y,z=z}
	setmetatable(v,Vector3MetaTable)
	return v
	]]
end

function Vector4(x,y,z,w)
	return {x=x,y=y,z=z,w=w}
end

function Rect(x,y,w,h)
	return {x=x,y=y,w=w,h=h}
end

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