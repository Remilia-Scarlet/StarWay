#pragma once
#define FS_INCLUDE

#if defined(TINY_PLATFORM_WINDOWS)
#include "ms/File_Win.h"
#include "ms/Path_Win.h"
#else
#error Platform unknown
#endif