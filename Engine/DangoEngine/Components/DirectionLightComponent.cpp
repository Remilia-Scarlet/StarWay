#include "DangoEngine/precomp.h"
#include "DirectionLightComponent.h"
#include "TransformComponent.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\Manager\ConstantBufferManager.h"

DirectionLightComponentPtr DirectionLightComponent::create(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
{
	DirectionLightComponent* ret = new DirectionLightComponent();
	if (ret && ret->init(ambient, diffuse, specular))
		return DirectionLightComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return DirectionLightComponentPtr();
}

void DirectionLightComponent::render()
{
	if (!_owner.isValid())
		return;
	TransformComponentPtr trans = _owner.lock()->getComponent<TransformComponent>();
	if (!trans.isValid())
		return;
	_light.direction = Vector3(0,0,1).rotateInPlace(trans->getRotation());
	LightManager::instance()->applyDirectionLight(_light);
}

bool DirectionLightComponent::prepareRenderShadowMap()
{
	if (!_shadowMapBuffer.isValid() || !_owner.isValid())
		return false;

	TransformComponentPtr trans = _owner.lock()->getComponent<TransformComponent>();
	if (!trans.isValid())
		return false;

	// set world matrix
	Matrix4 worldMat = trans->getNodeToParentMatrix();
	ConstantBufferManager::instance()->getVsGlobalConstant().setWorldMatrix(worldMat);

	// set view matrix
	Matrix4 viewMatrix = trans->getParentToNodeMatrix();
	ConstantBufferManager::instance()->getVsGlobalConstant().setViewMatrix(viewMatrix);

	// set proj matrix
	Matrix4 projMatrix = CreateOrthographicProjMatrix((float)SHADOW_MAP_WIDTH, (float)SHADOW_MAP_HEIGHT, 0.02f, 400.f);
	ConstantBufferManager::instance()->getVsGlobalConstant().setProjectionMatrix(projMatrix);

	GraphicMgr::instance()->setRenderBuffer(_shadowMapBuffer);
	return true;
}

void DirectionLightComponent::finishedRenderShadowMap()
{
	GraphicMgr::instance()->resetRenderBuffer();
}

const GfxRenderBufferPtr& DirectionLightComponent::getShadowMapBuffer()
{
	return _shadowMapBuffer;
}

DirectionLightComponent::DirectionLightComponent()
	:BaseComponent(TO_STRING(DirectionLightComponent))
{
}

DirectionLightComponent::~DirectionLightComponent()
{
}

bool DirectionLightComponent::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
{
	do
	{
		_light.ambient = ambient;
		_light.diffuse = diffuse;
		_light.specular = specular;
		_shadowMapBuffer = GfxRenderBuffer::create(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		TINY_BREAK_IF(!_shadowMapBuffer.isValid());
		return true;
	} while (0);
	TinyAssert(false);
	return false;
}

void DirectionLightComponent::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);
	if (owner.isValid())
	{
		owner->setFlag(ObjectFlag::IS_LIGHT, true);
		owner->setFlag(ObjectFlag::IS_DIRECTIONAL_LIGHT, true);
	}
}
