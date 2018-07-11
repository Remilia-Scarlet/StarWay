#pragma once
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Graphic/gfx/GfxShaderPixel.h"
#include "Math/vector/Vector.h"
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "Graphic/gfx/GfxInputLayout.h"
#include "Graphic/gfx/GfxTexture.h"
#include "Graphic/gfx/GfxRenderBuffer.h"

#if defined(TINY_PLATFORM_WINDOWS)
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
#if defined(TINY_PLATFORM_WINDOWS)
	static bool createInstance(int width, int height, HWND hWnd);
#else
	#error Unknown graphic engine
#endif
	static void destroyInstance();
	static PlatformGraphicMgr* instance();

	virtual void setViewPort(const ViewPort& viewPort) = 0;
	virtual ViewPort getViewPort() = 0;

	template<class VertexType>
	GfxInputLayoutPtr getInputLayout();

	virtual void setPixelShader(const GfxShaderPixelPtr& ps) = 0;
	virtual void setVertexShader(const GfxShaderVertexPtr& vs) = 0;

	virtual void preRender() = 0;
	virtual void render() = 0;

	virtual void draw() = 0;

	virtual void setDrawIndexNumber(int number) { _drawIndexNumber = number; }
	virtual int getDrawIndexNumber() { return _drawIndexNumber; }

	virtual void setRenderBuffer(const GfxRenderBufferPtr& buffer) = 0;
	virtual void resetRenderBuffer() = 0;
protected:
	GraphicMgr();
	virtual ~GraphicMgr();
	virtual void clearDevice() = 0;
	virtual GfxInputLayoutPtr initInputLayout(const VertexInputlayoutDescription& description) = 0;
	int _drawIndexNumber;
	std::unordered_map<std::string, GfxInputLayoutPtr> _inputLayouts;

	static GraphicMgr* s_instance;
};

template <class VertexType>
GfxInputLayoutPtr GraphicMgr::getInputLayout()
{
	auto it = _inputLayouts.find(VertexType::getName());
	if (it != _inputLayouts.end())
		return it->second;

	const VertexInputlayoutDescription& description = VertexType::getInputLayoutDescription();
	GfxInputLayoutPtr layout = initInputLayout(description);
	_inputLayouts[VertexType::getName()] = layout;
	return layout;
}

#if defined(TINY_PLATFORM_WINDOWS)
#include "Graphic/DX11/DX11GraphicMgr.h"
#else
#error Unknown graphic engine
#endif