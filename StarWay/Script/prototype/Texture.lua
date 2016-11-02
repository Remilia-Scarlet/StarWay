Texture = {}
Texture.__index = Texture
function Texture:create(textureFileName,shaderFileName)
	local obj = {}
	setmetatable(obj,self)
	obj.c_ref = CreateTexture(textureFileName,shaderFileName)
	return obj
end 
