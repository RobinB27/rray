cmake_minimum_required(VERSION 3.16)
project(rren C)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" FORCE)
endif()

find_package(SDL3 REQUIRED CONFIG)
add_executable(rren main.c
    lib/framebuf.c
    lib/vla.c
    lib/scene.c
    lib/raytracer.c
)

# math.h isn't in libc on linux, needs libm
if(NOT WIN32)
    target_link_libraries(rren PRIVATE m)
endif()

set(RREN_WARNINGS -Wall -Wextra -Wpedantic -Wshadow)
target_compile_options(rren PRIVATE
    "$<$<C_COMPILER_ID:GNU,Clang>:${RREN_WARNINGS}>"
    "$<$<C_COMPILER_ID:MSVC>:${/W4}>"
)

set(RREN_SANITIZERS -fsanitize=address,undefined -fno-omit-frame-pointer)
target_compile_options(rren PRIVATE "$<$<CONFIG:Debug>:${RREN_SANITIZERS}>")
target_link_options(rren PRIVATE "$<$<CONFIG:Debug>:${RREN_SANITIZERS}>")

target_link_libraries(rren PRIVATE SDL3::SDL3)