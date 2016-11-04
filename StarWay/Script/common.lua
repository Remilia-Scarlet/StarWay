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

function Vector3(x,y,z)
	return {x=x,y=y,z=z}
end
