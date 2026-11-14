cmake_minimum_required(VERSION 3.16)
project(rren C)

find_package(SDL3 REQUIRED CONFIG)
add_executable(rren main.c
    lib/framebuf.c
    lib/util.c
    lib/scene.c
    lib/raytracer.c
)

# math.h isn't in libc on linux, needs libm
if(NOT WIN32)
    target_link_libraries(rren PRIVATE m)
endif()

target_link_libraries(rren PRIVATE SDL3::SDL3)