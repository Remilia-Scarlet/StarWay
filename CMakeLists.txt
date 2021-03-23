cmake_minimum_required(VERSION 3.16)
project (StarWay)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

#UnitTest
add_subdirectory (Engine/UnitTest)

#Ash
add_subdirectory (Engine/Ash)

#DangoEngine
add_subdirectory (Engine/DangoEngine)

#WinLauncher
add_subdirectory (Engine/WinLauncher)

#Math
add_subdirectory (Engine/DangoMath)



#StarWay
add_subdirectory (StarWay)