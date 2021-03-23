#include "DangoEngine/precomp.h"
#include "MaterialComponent.h"
#include <fstream>
#include "Graphic\Manager\ShaderMgr.h"
#include "Graphic\Manager\DefaultMgr.h"
#include "Ash/FileSystem/fs_include.h"

MaterialComponentPtr MaterialComponent::create( const std::string& fileName)
{
	MaterialComponent* ret = new MaterialComponent();
	if (!ret || !ret->init(fileName))
		TINY_SAFE_DELETE(ret);
	return MaterialComponentPtr(ret);
}

MaterialComponentPtr MaterialComponent::create()
{
	MaterialComponent* ret = new MaterialComponent();
	if (!ret || !ret->init(""))
		TINY_SAFE_DELETE(ret);
	return MaterialComponentPtr(ret);
}

void MaterialComponent::render()
{
	/*if (_gfxTexture.isValid())
		_gfxTexture->render();
	else
		DefaultMgr::instance()->getDefaultTexture()->render();

	if (_gfxMaterial.isValid())
		_gfxMaterial->render();
	else
		DefaultMgr::instance()->getDefaultMaterial()->render();

	if (_psShader.isValid())
		_psShader->render();
	else if (_gfxTexture.isValid())
		DefaultMgr::instance()->getDefaultPSWithTex()->render();
	else
		DefaultMgr::instance()->getDefaultPSWithNoTex()->render();*/
}

void MaterialComponent::setMaterial(const MaterialPtr& material)
{
	_material = material;
}

const MaterialPtr& MaterialComponent::getMaterial() const
{
	return _material;
}

void MaterialComponent::setWireFrame(bool wireFrame)
{
	TinyAssert(false);//todo
	//if (_gfxTexture == nullptr || _gfxTexture == DefaultMgr::instance()->getDefaultTexture())
	//{
	//	_gfxTexture = GfxTexture::create();
	//}
	//_gfxTexture->setWireFrame(wireFrame);
}

bool MaterialComponent::isWireFrame()
{
	TinyAssert(false);//todo
	//if (_gfxTexture == nullptr)
	//	return DefaultMgr::instance()->getDefaultTexture()->isWireFrame();
	//return _gfxTexture->isWireFrame();
	return false;
}

bool MaterialComponent::init(const std::string& fileName)
{
	do 
	{
		_material = Material::create(fileName);
		TINY_BREAK_IF(!_material.isValid());
		return true;
	} while (0);
	TinyAssert(false,"MaterialComponent::init failed");
	return false;
}

MaterialComponent::MaterialComponent()
	:BaseComponent(TO_STRING(MaterialComponent))
{
}

MaterialComponent::~MaterialComponent()
{
}

