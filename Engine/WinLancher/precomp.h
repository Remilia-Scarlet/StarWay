#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Settings.h"
#if TINY_PLATFORM_TARGET != TINY_PLATFORM_WINDOWS
#error This project is only for Windows, please set DurangoLancher as default start project
#endif

#include <memory>
#include <string>
#include <vector>
#include <map>