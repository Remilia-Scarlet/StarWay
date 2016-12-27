#include "Graphic\precomp.h"
#include "GfxMaterial.h"
#include "Graphic\Manager\ConstantBufferManager.h"

GfxMaterial::GfxMaterial()
{

}

GfxMaterial::~GfxMaterial()
{

}

GfxMaterialPtr GfxMaterial::create(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emit)
{
	GfxMaterial* ret = new GfxMaterial();
	if (ret && ret->init(ambient, diffuse, specular, emit))
	{
		return GfxMaterialPtr(ret);
	}
	TINY_SAFE_DELETE(ret);
	return GfxMaterialPtr();
}

GfxMaterialPtr GfxMaterial::create()
{
	GfxMaterial* ret = new GfxMaterial();
	if (ret && ret->init(Vector4(), Vector4(), Vector4(), Vector4()))
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

const Vector4& GfxMaterial::getEmit()
{
	return _emit;
}

void GfxMaterial::setEmit(const Vector4& emit)
{
	_emit = emit;
}

void GfxMaterial::render()
{
	ConstantBufferManager::instance()->setPSVector(1, _ambient);
	ConstantBufferManager::instance()->setPSVector(2, _diffuse);
	ConstantBufferManager::instance()->setPSVector(3, _specular);
}

bool GfxMaterial::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emit)
{
	do 
	{
		_ambient = ambient;
		_diffuse = diffuse;
		_specular = specular;
		_emit = emit;
		return true;
	} while (0);
	return false;
}

