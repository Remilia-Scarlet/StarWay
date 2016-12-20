#include "Graphic\precomp.h"
#include "GfxMaterial.h"
#include "Graphic\Manager\ConstantBufferManager.h"

GfxMaterial::GfxMaterial()
{

}

GfxMaterial::~GfxMaterial()
{

}

GfxMaterialPtr GfxMaterial::create(Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	GfxMaterial* ret = new GfxMaterial();
	if (ret && ret->init(ambient, diffuse, specular))
	{
		return GfxMaterialPtr(ret);
	}
	TINY_SAFE_DELETE(ret);
	return GfxMaterialPtr();
}

GfxMaterialPtr GfxMaterial::create()
{
	GfxMaterial* ret = new GfxMaterial();
	if (ret && ret->init(Vector4(), Vector4(), Vector4()))
	{
		return GfxMaterialPtr(ret);
	}
	TINY_SAFE_DELETE(ret);
	return GfxMaterialPtr();
}

const Vector4& GfxMaterial::getAmbient()
{
	return _ambient;
}

void GfxMaterial::setAmbient(const Vector4& ambient)
{
	_ambient = ambient;
}

const Vector4& GfxMaterial::getDiffuse()
{
	return _diffuse;
}

void GfxMaterial::setDiffuse(const Vector4& diffuse)
{
	_diffuse = diffuse;
}

const Vector4& GfxMaterial::getSpecular()
{
	return _specular;
}

void GfxMaterial::setSpecular(const Vector4& specular)
{
	_specular = specular;
}

void GfxMaterial::render()
{
	ConstantBufferManager::instance()->setPSVector(13, _ambient);
	ConstantBufferManager::instance()->setPSVector(14, _diffuse);
	ConstantBufferManager::instance()->setPSVector(15, _specular);
}

bool GfxMaterial::init(Vector4 ambient, Vector4 diffuse, Vector4 specular)
{
	do 
	{
		_ambient = ambient;
		_diffuse = diffuse;
		_specular = specular;
		return true;
	} while (0);
	return false;
}

