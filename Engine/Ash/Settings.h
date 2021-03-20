#pragma once

#if defined(_WINDOWS)
	#define TINY_PLATFORM_WINDOWS
#else
	#error Unknown Platform
#endif

//set TINY_GRAPHIC_ENGINE_TARGET
#ifdef TINY_PLATFORM_WINDOWS
	#define TINY_GRAPHIC_ENGINE_DX11
#else
#error Unsupport platform
#endif

#ifndef NDEBUG
	#define TINY_DEBUG
#else
	#define TINY_RETAIL
#endif