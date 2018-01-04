#pragma once
#include <string>
#include <cassert>
#include <cstdarg>
#include <memory>

#ifdef _DEBUG

// Useage:
// TinyAssert(expression) or TinyAssert(expression,format,...)
// Example: 
// TinyAssert(1 + 1 == 2);	// No information
// TinyAssert(1 + 1 == 3, "%d + %d is not equal %d", 1, 1, 3); //Assert with information
#define TinyAssert(...) TINY_ASSERT_(MY_FILE, MY_LINE, __VA_ARGS__)



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
			void DebugString(const char*, ...);    \
			DebugString(                           \
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
			void DebugString(const char*, ...);                 \
			DebugString(                                        \
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

#else //#ifdef _DEBUG
#define TinyAssert(...) do{}while(0)
#endif