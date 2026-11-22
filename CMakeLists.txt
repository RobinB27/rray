cmake_minimum_required(VERSION 3.10)
project(rray C)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" FORCE)
endif()

find_package(SDL3 REQUIRED CONFIG)
find_package(OpenMP QUIET COMPONENTS C)

add_executable(rray main.c
    lib/util.c
    lib/framebuf.c
    lib/vla.c
    lib/scene.c
    lib/raytracer.c
)

# math.h isn't in libc on linux, needs libm
if(NOT WIN32)
    target_link_libraries(rray PRIVATE m)
endif()

set(RRAY_WARNINGS -Wall -Wextra -Wpedantic -Wshadow)
target_compile_options(rray PRIVATE
    "$<$<C_COMPILER_ID:GNU,Clang>:${RRAY_WARNINGS}>"
    "$<$<C_COMPILER_ID:MSVC>:/W4>"
)

set(RRAY_SANITIZERS -fsanitize=address,undefined -fno-omit-frame-pointer)
target_compile_options(rray PRIVATE "$<$<CONFIG:Debug>:${RRAY_SANITIZERS}>")
target_link_options(rray PRIVATE "$<$<CONFIG:Debug>:${RRAY_SANITIZERS}>")

if(OpenMP_C_FOUND)
    target_link_libraries(rray PRIVATE OpenMP::OpenMP_C)
endif()
target_link_libraries(rray PRIVATE SDL3::SDL3)