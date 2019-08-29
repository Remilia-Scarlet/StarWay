cmake_minimum_required(VERSION 3.8.2)
project (UnitTest) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Yuprecomp.h")
    set_source_files_properties(precomp.cpp PROPERTIES COMPILE_FLAGS "/Yc")
endif(MSVC)

include_directories(..)
include_directories(../../tools/googletest/src/googletest/include)

add_executable(UnitTest ${SOURCE_FILES})
target_compile_features(UnitTest PUBLIC cxx_std_17)