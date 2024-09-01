# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.20)
# Setup project build setup
include(${CMAKE_SOURCE_DIR}/cmake/compiler_config.cmake)
# Project root label
project(EthosPlotting)

# Add sub directories to build
add_subdirectory(Core)
