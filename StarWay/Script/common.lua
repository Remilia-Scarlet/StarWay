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

Vector3MetaTable = {
	__add = function(a,b)
		return {a.x + b.x, a.y + b.y, a.z + b.z}
	end,
	__sub = function(a,b)
		return {a.x - b.x, a.y - b.y, z.z - b.z}
	end
}

function Vector3(x,y,z)
	local v = {x=x,y=y,z=z}
	setmetatable(v,Vector3MetaTable)
	return v
end

function Vector4(x,y,z,w)
	return {x=x,y=y,z=z,w=w}
end

function Rect(x,y,w,h)
	return {x=x,y=y,w=w,h=h}
end