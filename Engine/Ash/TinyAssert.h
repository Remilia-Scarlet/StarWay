#pragma once
#include <string>
#include <assert.h>
#include <cstdarg>
#include <memory>

#ifdef TINY_RETAIL
#define TinyAssert(...) do{}while(0)

#define ScopeFlagAssert(...) do{}while(0) 
#define BeginFlagAssert(...) do{}while(0)
#define EndpeFlagAssert(...) do{}while(0)
#else //#ifndef TINY_RETAIL

// Useage:
// TinyAssert(expression) or TinyAssert(expression,format,...)
// Example: 
// TinyAssert(1 + 1 == 2);	// No information
// TinyAssert(1 + 1 == 3, "%d + %d is not equal %d", 1, 1, 3); //Assert with information
#define TinyAssert(...) TINY_ASSERT_(MY_FILE, MY_LINE, __VA_ARGS__)

//Ensure when entering the scope, FLAG is false. Then set FLAG to true, and when exiting the scope, set FLAG back to false.
#define ScopeFlagAssert(FLAG)  ScopeFlagAssert_(MY_LINE, FLAG)
#define BeginFlagAssert(FLAG) do{TinyAssert(!FLAG, "ScopeFlagAssert failed!"); FLAG = true;}while(0)
#define EndpeFlagAssert(FLAG) do{(FLAG) = false;}while(false)


//////////////////////////////////////////////////////////IMPLEMENTATION///////////////////////////////////////////////
//Platform related defines
#define MY_ASSERT assert
#define MY_FILE __FILE__
#define MY_LINE __LINE__

#define TINY_ASSERT_NO_DEFIEN

#define TINY_ASSERT_CAT(lhs,rhs) lhs##rhs

#define TINY_ASSERT_APPLY_ASSERT(AST,...) TINY_ASSERT_APPLY_ASSERT_(AST,(__VA_ARGS__))
#define TINY_ASSERT_APPLY_ASSERT_(AST,ARGS) AST ARGS

#define TINY_ASSERT_(FILE_NAME, LINE, ...) TINY_ASSERT_APPLY_ASSERT(TINY_ASSERT_CAT, TINY_ASSERT_, TINY_ASSERT_HAS_ARG(__VA_ARGS__))(FILE_NAME, LINE, __VA_ARGS__)

#define TINY_ASSERT_0(FILE_NAME, LINE, EXPRESSION) \
	do                                             \
	{                                              \
		if (!!(EXPRESSION))                        \
		{                                          \
		}                                          \
		else                                       \
		{                                          \
			void Ash::DebugString(const char*, ...);    \
			Ash::DebugString(                           \
				"\n\n"                             \
				"********************\n"           \
				"TinyAssert failed!\n"             \
				"--\n"                             \
				"File:%s\n"                        \
				"Line:%d\n"                        \
				"Expression:%s\n"                  \
				"********************\n",          \
				FILE_NAME, LINE, #EXPRESSION);     \
			MY_ASSERT(EXPRESSION);                 \
		}                                          \
	} while (0)

#define TINY_ASSERT_1(FILE_NAME, LINE, EXPRESSION, FORMAT, ...) \
	do                                                          \
	{                                                           \
		if (!!(EXPRESSION))                                     \
		{                                                       \
		}                                                       \
		else                                                    \
		{                                                       \
			void Ash::DebugString(const char*, ...);                 \
			Ash::DebugString(                                        \
				"\n\n"                                          \
				"********************\n"                        \
				"TinyAssert failed!\n"                          \
				"--\n"                                          \
				"File:%s\n"                                     \
				"Line:%d\n"                                     \
				"Expression:%s\n"                               \
				"Message:"##FORMAT##"\n",                       \
				FILE_NAME, LINE, #EXPRESSION, __VA_ARGS__);     \
			MY_ASSERT(EXPRESSION);                              \
		}                                                       \
	} while (0)

#define TINY_ASSERT_HAS_ARG(...) TINY_ASSERT_APPLY_ASSERT(TINY_ASSERT_ARGS_NUM,__VA_ARGS__,\
	1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,0,0)

#define TINY_ASSERT_ARGS_NUM(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,\
	_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,...) _30

#define ScopeFlagAssert_(LINE, FLAG) \
struct TINY_ASSERT_CAT(__ScopeEnsureAssertStruct_,LINE) \
{\
	TINY_ASSERT_CAT(__ScopeEnsureAssertStruct_,LINE)(std::atomic_bool& flag) :_flag(flag)\
	{\
		BeginFlagAssert(_flag); \
	}\
	~TINY_ASSERT_CAT(__ScopeEnsureAssertStruct_,LINE)()\
	{\
		EndpeFlagAssert(_flag); \
	}\
private:\
	std::atomic_bool& _flag;\
};\
TINY_ASSERT_CAT(__ScopeEnsureAssertStruct_,LINE) TINY_ASSERT_CAT(__ScopeEnsureAssertInst_, LINE) {FLAG}

#endif