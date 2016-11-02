function CppCallLua(tab,...)
	local callFun = _G
	for i,f in ipairs(tab) do
		callFun = callFun[f]
	end
	if(callFun ~= _G and callFun ~= nil) then
		callFun(...)
	end
end