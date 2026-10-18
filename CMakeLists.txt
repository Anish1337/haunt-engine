cmake_minimum_required(VERSION 3.20)
# Name + C++
project(haunt-engine LANGUAGES CXX)
# Enable C++ 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Locates V hdr/lib
find_package(Vulkan REQUIRED)
# Locate glfw
find_package(glfw3 REQUIRED)
# creates exec (relative path)
add_executable(haunt src/main.cpp)

target_link_libraries(haunt PRIVATE
    Vulkan::Vulkan
glfw)
