#pragma once

#if defined(PLATFORM_WIN64) || defined(PLATFORM_WIN32)
	#define TINY_PLATFORM_WINDOWS
#elif defined(PLATFORM_DURANGO)
	#define TINY_PLATFORM_DURANGO
#elif defined(PLATFORM_ORBITS)
	#define TINY_PLATFORM_ORBITS
#else
	#error Unknown Platform
#endif


//set TINY_GRAPHIC_ENGINE_TARGET
#ifdef TINY_PLATFORM_WINDOWS
	#define TINY_GRAPHIC_ENGINE_DX11
#elif defined(TINY_PLATFORM_DURANGO)
	#define TINY_GRAPHIC_ENGINE_DX11
#elif defined(TINY_PLATFORM_ORBITS)
	#error todo
#else
#error Unsupport platform
#endif

//set  BitWide
#if defined(PLATFORM_WIN64) || defined(PLATFORM_DURANGO) || defined(PLATFORM_ORBITS)
	#define TINY_BITWIDE_X64
#elif defined(PLATFORM_WIN32)
	#define TINY_BITWIDE_X86
#else
	#error Unsupport platform
#endif