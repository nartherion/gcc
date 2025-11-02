cmake_minimum_required(VERSION 3.21)
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 23)
endif()

project(nartherion-gcc)
set(NARTHERION_GCC_SOURCE_DIR ${nartherion-gcc_SOURCE_DIR})
add_subdirectory(src)
