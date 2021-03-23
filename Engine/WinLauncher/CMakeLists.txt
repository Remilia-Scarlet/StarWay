cmake_minimum_required(VERSION 3.16)
project (WinLauncher) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_executable(WinLauncher ${SOURCE_FILES})

target_include_directories(WinLauncher PUBLIC ..)

# pch
target_precompile_headers(WinLauncher PUBLIC PreComp.h)

target_link_libraries (WinLauncher StarWay)
