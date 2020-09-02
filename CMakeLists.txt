cmake_minimum_required(VERSION 3.10)

# set the project name
project(raytracer)

# include directories
include_directories(include)

# C++ flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add the executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(raytracer ${SOURCES})

# Libraries
target_link_libraries(raytracer pthread)