#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Ash/TinyAssert.h"
#include <cstdint>

#define TINY_DEFINE_PTR(CALSSNAME) class CALSSNAME;typedef RefCountPtr<CALSSNAME> CALSSNAME##Ptr;typedef WeakRefPtr<CALSSNAME> CALSSNAME##WeakPtr
#define TINY_BREAK_IF(CENTENCE) if(CENTENCE)break
#define TINY_SAFE_DELETE(PTR) do{if(PTR != nullptr){delete PTR;PTR = nullptr;}}while(0)
#define TINY_SAFE_RELEASE(PTR) do{if(PTR != nullptr){PTR->Release();PTR = nullptr;}}while(0)

#define TO_STRING(_NAME_) (#_NAME_)

typedef int64_t ObjectID;
const static ObjectID INVALID_OBJECT_ID = 0;

inline ObjectID genericObjectId() { static ObjectID id = INVALID_OBJECT_ID; return ++id; }