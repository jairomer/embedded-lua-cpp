cmake_minimum_required(VERSION 3.7)

project(cppcustom-lua-triggers)

# So we can add the LUA_TUTORIAL_DEBUG preprocessor define and other flags
# to stay in debug mode - see https://cmake.org/Wiki/CMake_Useful_Variables#Compilers_and_Tools
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DLUA_TUTORIAL_DEBUG" )


if(MSVC)
    # Use multiple processors when building
    add_compile_options(/MP)
    # Warning level 4, all warnings are errors
	add_compile_options(/W4 /wd4201 /WX)
else()
    # All Warnings, all warnings are errors
	add_compile_options(-W -Wall -Werror -g)
endif()

set (PROJECT_SOURCES
        "src/main.cpp"
        )

add_subdirectory(3pp/lua-5.3.5)

add_executable(runme ${PROJECT_SOURCES})

target_link_libraries(runme PUBLIC LuaLib)

