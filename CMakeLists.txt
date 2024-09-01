# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.20)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_config.cmake)

project(EthosPlotting)

# Add sub directories to build
add_subdirectory(core)
