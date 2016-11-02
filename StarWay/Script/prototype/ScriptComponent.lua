ScriptComponent = {}
ScriptComponent.__index = ScriptComponent
function ScriptComponent:create(scriptTabName)
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateScriptComponent(scriptTabName)
	return obj
end 