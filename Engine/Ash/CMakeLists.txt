cmake_minimum_required(VERSION 3.16)
project (Ash) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(Ash STATIC ${SOURCE_FILES})

target_include_directories(Ash PUBLIC ../Extern/boost)
target_include_directories(Ash PUBLIC ..)

# pch
target_precompile_headers(Ash PUBLIC PreComp.h)

target_compile_features(Ash PUBLIC cxx_std_17)