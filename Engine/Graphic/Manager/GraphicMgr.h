#pragma once
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Graphic/gfx/GfxShaderPixel.h"
#include "Math/vector/Vector.h"
#include "Graphic/Vertex/InputLayoutDefine.h"

#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	class DX11GraphicMgr;
	typedef DX11GraphicMgr PlatformGraphicMgr;
#else
	#error Unknown graphic engine
#endif

class ViewPort
{
public:
	ViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MinDepth, float MaxDepth)
		:TopLeftX(TopLeftX), TopLeftY(TopLeftY), Width(Width), Height(Height), MinDepth(MinDepth), MaxDepth(MaxDepth) {}
	ViewPort() { memset(this, 0, sizeof(ViewPort)); }
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
} ;


struct InputLayout_COMMON
{
	InputLayout_COMMON() {}
	Vector4 _position;
	Vector2 _texcoodin;
};


class GraphicMgr
{
public:
#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	static bool createInstance(int width, int height, HWND hWnd);
#else
	#error Unknown graphic engine
#endif
	static void destroyInstance();
	static PlatformGraphicMgr* instance();

	virtual void setViewPort(const ViewPort& viewPort) = 0;
	virtual ViewPort getViewPort() = 0;

	virtual void setInputLayout(InputLayoutType inputLayoutType) = 0;

	virtual void setPixelShader(const GfxShaderPixelPtr& ps) = 0;
	virtual void setVertexShader(const GfxShaderVertexPtr& vs) = 0;

	virtual void preRender() = 0;
	virtual void render() = 0;

	virtual void draw() = 0;

	virtual void setDrawIndexNumber(int number) { _drawIndexNumber = number; }
	virtual int getDrawIndexNumber() { return _drawIndexNumber; }
protected:
	GraphicMgr();
	virtual ~GraphicMgr();
	virtual void clearDevice() = 0;
	int _drawIndexNumber;

	static GraphicMgr* s_instance;
};

#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
#include "Graphic/DX11/DX11GraphicMgr.h"
#else
#error Unknown graphic engine
#endif