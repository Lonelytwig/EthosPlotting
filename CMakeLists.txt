# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.20)
# Setup project build setup
include(${CMAKE_SOURCE_DIR}/cmake/compiler_config.cmake)
# Project root label
project(EthosPlotting)

# Define output directory paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)

# Add sub directories to build
add_subdirectory(Core)
