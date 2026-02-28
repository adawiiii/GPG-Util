cmake_minimum_required(VERSION 3.16)
project(GPG-Util LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIREED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(GPG-Util src/main.cpp)

if (MSVC)
  target_compile_options(GPG-Util PRIVATE /W4)
else()
  target_compile_options(GPG-Util PRIVATE -Wall -Wextra -Wpedantic)
endif()
