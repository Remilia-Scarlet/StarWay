cmake_minimum_required(VERSION 3.16)
project (DangoEngine) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(DangoEngine STATIC ${SOURCE_FILES})

target_include_directories(DangoEngine PUBLIC ..)

# pch
target_precompile_headers(DangoEngine PUBLIC PreComp.h)

target_link_libraries (DangoEngine Ash)
target_link_libraries (DangoEngine DangoMath)

target_compile_features(DangoEngine PUBLIC cxx_std_17)