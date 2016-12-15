#include "TinyEngine/precomp.h"
#include "TinyEngine/Graphic/WavefrontObjReader.h"

WavefrontObjReader* WavefrontObjReader::instance()
{
	static WavefrontObjReader s_instance;
	return &s_instance;
}
