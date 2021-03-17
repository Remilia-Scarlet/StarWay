cmake_minimum_required(VERSION 3.8.2)
project (Ash) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /YuAsh/AshCore.h")
    set_source_files_properties(AshCore.cpp PROPERTIES COMPILE_FLAGS "/Yc")
endif(MSVC)

include_directories(..)
include_directories(../Extern/boost)

add_library(Ash STATIC ${SOURCE_FILES})
target_compile_features(Ash PUBLIC cxx_std_17)