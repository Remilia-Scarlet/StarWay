cmake_minimum_required(VERSION 3.8.2)
project (StarWay) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_library(StarWay STATIC ${SOURCE_FILES})

target_include_directories(StarWay PUBLIC .)

# pch
target_precompile_headers(StarWay PUBLIC PreComp.h)

target_link_libraries (StarWay DangoEngine)

