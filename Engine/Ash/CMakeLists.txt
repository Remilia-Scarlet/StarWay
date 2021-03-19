cmake_minimum_required(VERSION 3.16)
project (Ash) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

include_directories(..)
include_directories(../Extern/boost)

add_library(Ash STATIC ${SOURCE_FILES})

# pch
target_precompile_headers(Ash PRIVATE PreComp.h)

target_compile_features(Ash PUBLIC cxx_std_17)