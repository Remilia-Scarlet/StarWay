cmake_minimum_required(VERSION 3.8.2)
project (UnitTest) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

include_directories(${PROJECT_SOURCE_DIR}/../../Tools/GoogleTest/src/googletest/include)
#todo: platform
link_directories(${PROJECT_SOURCE_DIR}/../../Tools/GoogleTest/build/Win64)

add_executable(UnitTest ${SOURCE_FILES})

# pch
target_precompile_headers(UnitTest PRIVATE PreComp.h)

target_compile_features(UnitTest PUBLIC cxx_std_17)

target_link_libraries (UnitTest Ash)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
	target_link_libraries (UnitTest gtestd)
	target_link_libraries (UnitTest gtest_maind)
else()
	target_link_libraries (UnitTest gtest)
	target_link_libraries (UnitTest gtest_main)
endif()