#include "Graphic\precomp.h"
#include "GfxMaterial.h"

GfxMaterial::GfxMaterial()
{

}

GfxMaterial::~GfxMaterial()
{

}

GfxMaterialPtr GfxMaterial::create(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector4 reflect)
{
	GfxMaterial* ret = new GfxMaterial();
	if (ret && ret->init(ambient, diffuse, specular, reflect))
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

const Vector4& GfxMaterial::getReflect()
{
	return _reflect;
}

void GfxMaterial::setReflect(const Vector4& reflect)
{
	_reflect = reflect;
}

bool GfxMaterial::init(Vector4 ambient, Vector4 diffuse, Vector4 specular, Vector4 reflect)
{
	do 
	{
		_ambient = ambient;
		_diffuse = diffuse;
		_specular = specular;
		_reflect = reflect;
		return true;
	} while (0);
	return false;
}

