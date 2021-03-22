cmake_minimum_required(VERSION 3.8.2)
project (UnitTest) 

file(GLOB_RECURSE SOURCE_FILES FOLLOW_SYMLINKS CONFIGURE_DEPENDS *.cpp *.c *.h *.natvis *.inl)
source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCE_FILES})

add_executable(UnitTest ${SOURCE_FILES})

# pch
target_precompile_headers(UnitTest PRIVATE PreComp.h)

#target_compile_features(UnitTest PUBLIC cxx_std_17)

target_link_libraries (UnitTest Ash)

#google test
set(GTestLocation ${PROJECT_SOURCE_DIR}/../../Tools/GoogleTest/build/Win64)
add_library(GTest STATIC IMPORTED )
add_library(GTestMain STATIC IMPORTED )
set_target_properties(GTest PROPERTIES
	IMPORTED_LOCATION ${GTestLocation}/gtest.lib
	IMPORTED_LOCATION_DEBUG ${GTestLocation}/gtestd.lib)
set_target_properties(GTestMain PROPERTIES
	IMPORTED_LOCATION ${GTestLocation}/gtest_main.lib
	IMPORTED_LOCATION_DEBUG ${GTestLocation}/gtest_maind.lib)
target_include_directories(GTest INTERFACE ${PROJECT_SOURCE_DIR}/../../Tools/GoogleTest/src/googletest/include)

target_link_libraries (UnitTest GTest)
target_link_libraries (UnitTest GTestMain)

