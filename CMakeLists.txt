cmake_minimum_required(VERSION 3.10)

# set the project name
project(raytracer)

include_directories(include)

# add the executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(raytracer ${SOURCES})