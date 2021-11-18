cmake_minimum_required(VERSION 3.20)
project(mdrd C)
set(CMAKE_C_FLAGS_DEBUG
        "${CMAKE_C_FLAGS_DEBUG} -g3 -O0 -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG
        "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

set(CMAKE_C_STANDARD 99)

add_executable(mdrd main.c)

include(FindPkgConfig)
pkg_check_modules(FCgi REQUIRED IMPORTED_TARGET "fcgi")
pkg_check_modules(CMark REQUIRED IMPORTED_TARGET "libcmark")
target_link_libraries(mdrd PRIVATE fcgi cmark)