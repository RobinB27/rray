cmake_minimum_required(VERSION 3.16)
project(rren C)

find_package(SDL3 REQUIRED CONFIG)
add_executable(rren main.c
    lib/framebuf.c
)
target_link_libraries(rren PRIVATE SDL3::SDL3)