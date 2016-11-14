SplineCamera = {
	splineIndex = 0,
	freeCamera = nil,
	spline = nil,
	t = 0.0,
	controlPoints = {},
	splineLine = nil,
	currentSpline = nil
}

SplineCamera.init = function()
	SplineCamera:changeSpline()
end

function SplineCamera:changeSpline()
--Print("changeSpline",self.splineIndex)
	self.t = 0
	if(self.splineIndex == 0)then
		self:setCubicSpline()
	elseif(self.splineIndex == 1)then
		self:setNurbs()
	elseif(self.splineIndex == 2)then
		self:setBezier()
	end
	self.splineIndex = self.splineIndex + 1
	if(self.splineIndex == 3)then
		self.splineIndex = 0
	end
end

function SplineCamera:setCubicSpline()
--Print("setCubicSpline")
	local scene = GetCurrentScene()
	self:removeObj()
	local ctlPoints = {Vector3(0,0,0),Vector3(2,5,0),Vector3(10,0,0),Vector3(10,-5,0),Vector3(14,-3,0),Vector3(10,2,0)}
	for _,v in pairs(ctlPoints) do
		local controlPoint = CreateSimpleSphereOnPos(v)
		table.insert(self.controlPoints,controlPoint)
		scene:addObject(controlPoint)
	end
	self.currentSpline = Spline:create(ctlPoints)
	self:createSplineLine()
end

function SplineCamera:setNurbs()
--Print("setNurbs")
	local scene = GetCurrentScene()
	self:removeObj()
	local ctr = {Vector3(6.0,  0.0,  6.0),
					   Vector3(-5.5,  0.5,  5.5),
					   Vector3(-5.0,  1.0, -5.0),
					   Vector3( 4.5,  1.5, -4.5),
					   Vector3( 4.0,  2.0,  4.0),
					   Vector3(-3.5,  2.5,  3.5),
					   Vector3(-3.0,  3.0, -3.0),
					   Vector3( 2.5,  3.5, -2.5),
					   Vector3( 2.0,  4.0,  2.0),
					   Vector3(-1.5,  4.5,  1.5),
					   Vector3(-1.0,  5.0, -1.0),
					   Vector3( 0.5,  5.5, -0.5),
					   Vector3( 0.0,  6.0,  0.0)}
	self.currentSpline = Nurbs:create(4,ctr,{0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 11, 11},{1,1,1,1,1,1,1,1,1,1,1,1,1})
	for _,v in pairs(ctr) do
		local obj = CreateSimpleSphereOnPos(v)
		table.insert(self.controlPoints,obj)
		scene:addObject(obj)
	end
	self:createSplineLine()
end

function SplineCamera:setBezier()
--Print("setBezier")
	local scene = GetCurrentScene()
	self:removeObj()
	local ctr = {Vector3(0,0,0),Vector3(-1,5,0),Vector3(9,5,0),Vector3(15,0,0)}
	self.currentSpline = Bezier:create(3,ctr)
	for _,v in pairs(ctr) do
		local obj = CreateSimpleSphereOnPos(v)
		table.insert(self.controlPoints,obj)
		scene:addObject(obj)
	end
	self:createSplineLine()
end

function SplineCamera:removeObj()
	local currentScene = GetCurrentScene()
	for _,v in pairs(self.controlPoints) do
		currentScene:removeObject(v)
	end
	self.controlPoints = {}
	if(self.splineLine ~= nil)then
		currentScene:removeObject(self.splineLine)
		self.splineLine = nil
	end
end

SplineCamera.update = function(dt)
	local self = SplineCamera
	local obj = self.obj
	self:handleKeyChangeCamera(dt)
	local pos = self.currentSpline:get(self.t)
	self.t = self.t + 1/60/5
	if(self.t > 1)then
		self.t = 0
	end
	local transformCom = obj:getComponent("TransformComponent")
	local offset = Vector3(0,0,-3)
	transformCom:setLocation(pos + offset)
end

function SplineCamera:createSplineLine()
	local scene = GetCurrentScene()
	if(self.splineLine ~= nil)then
		scene:removeObj(self.splineLine)
		self.splineLine = nil
	end
	local vertex = {}
	local step = 0.001
	local t = 0
	while t <= 1.0 do
	--	Print(t)
		local pos = self.currentSpline:get(t)
		table.insert(vertex,{pos,Vector3(0,0,0),{x=0,y=0}} )
		t = t + step
		if(t >= 0.1 and t <= 0.9)then
			step = 0.01
		elseif(t > 0.9)then
			step = 0.001
		end
	end
--	Print(vertex)

	local obj = Object:create()
	self.splineLine = obj
	
	local meshConponent = MeshComponent:create(vertex,"ShaderFirst_VS.cso")
	meshConponent:setPrimitiveTopology(MeshComponent.LINE_STRIP)
	obj:addComponent(meshConponent)
	
	local texture = TextureComponent:create("","")
	obj:addComponent(texture)
	
	local transform = TransformComponent:create()
	obj:addComponent(transform)
	transform:setLocation(Vector3(0,0,0))
	
	scene:addObject(obj)
end


function SplineCamera:handleKeyChangeCamera(dt)
	local obj = SplineCamera.obj
	local btnR3Status = InputManager:getVitualBtnStatus(InputManager.BTN_R3)
	local btnPauseStatus = InputManager:getVitualBtnStatus(InputManager.BTN_Pause)
	local btnDownStatus = InputManager:getVitualBtnStatus(InputManager.BTN_DOWN)
	if(btnR3Status.isDown == true and btnR3Status.isChangedInThisFrame == true) then
		if(obj:getEnable() == true) then
			obj:setEnable(false)
			SplineCamera.freeCamera:setEnable(true)
		else
			obj:setEnable(true)
			SplineCamera.freeCamera:setEnable(false)
		end
	end
	if(btnPauseStatus.isDown == true and btnPauseStatus.isChangedInThisFrame == true) then
		obj:setEnable(false)
		SplineCamera.freeCamera:setEnable(true)
		self:setCubicSpline()
		self.splineIndex = 0
	end	
	if(btnDownStatus.isDown == true and btnDownStatus.isChangedInThisFrame == true) then
		if(obj:getEnable() == true)then
			self:changeSpline()
		end
	end
end