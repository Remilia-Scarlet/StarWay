cmake_minimum_required(VERSION 3.8.2)
project (DangoMath) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(DangoMath ${SOURCE_FILES})

target_include_directories(DangoMath PUBLIC ..)

# pch
target_precompile_headers(DangoMath PRIVATE PreComp.h)

