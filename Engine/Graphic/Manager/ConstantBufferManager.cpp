#include "Graphic/precomp.h"
#include "ConstantBufferManager.h"

ConstantBufferManager* ConstantBufferManager::s_instance = nullptr;
PlatformConstantBufferManager* ConstantBufferManager::instance()
{
	TinyAssert(s_instance != nullptr, "Call createInstance first");
	return static_cast<PlatformConstantBufferManager*>(s_instance);
}

bool ConstantBufferManager::createInstance()
{
	TinyAssert(s_instance == nullptr, "Call destroyInstance first");
#if defined(TINY_PLATFORM_WINDOWS)
	s_instance = new DX11ConstantBufferManager();
#else
#error unkonwn platform
#endif
	if (!s_instance || !s_instance->init())
	{
		TinyAssert(false, "Create ConstantBufferManager failed!");
		TINY_SAFE_DELETE(s_instance);
	}
	return s_instance != nullptr;
}

bool ConstantBufferManager::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
	return true;
}

void ConstantBufferManager::setLocalConstantVs(int constant, const void* data, size_t size)
{
	LocalConstantInfo info = getVsLocalParamInfo(constant);
	TinyAssert((int)std::ceil(static_cast<float>(size) / sizeof(Vector4)) + info._slot <= (int)_tempVSLocalBuffer.size());

	void* ptr = &_tempVSLocalBuffer[info._slot];
	memcpy(ptr, data, size);
}

void ConstantBufferManager::setLocalConstantPs(int constant, const void* data, size_t size)
{
	LocalConstantInfo info = getPsLocalParamInfo(constant);
	TinyAssert((int)std::ceil(static_cast<float>(size) / sizeof(Vector4)) + info._slot <= (int)_tempPSLocalBuffer.size());

	void* ptr = &_tempPSLocalBuffer[info._slot];
	memcpy(ptr, data, size);
}

ConstantBufferManager::ConstantBufferManager()
{

}


ConstantBufferManager::~ConstantBufferManager()
{
}
