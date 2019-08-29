cmake_minimum_required(VERSION 3.8.2)
project (StarWay)

# treat warnings as errors
if (MSVC)
    add_compile_options(/WX)
else()
    add_compile_options(-Werror)
endif()

#UnitTest
add_subdirectory (Engine/UnitTest)

#Ash
#include_directories ("${PROJECT_SOURCE_DIR}/Engine/Ash")
add_subdirectory (Engine/Ash)
set (EXTRA_LIBS ${EXTRA_LIBS} Ash)

