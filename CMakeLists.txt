# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.20)

project(MyApp)
set(CMAKE_CXX_STANDARD 17)

# Define output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)

# Specify module source directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Include other cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/ThirdPartyDependencies.cmake)

# Define the sources for your application
set(APP_SOURCES "${SRC_DIR}/main.cpp" "${imgui_SOURCE_DIR}/imgui_demo.cpp")

# Add executable
add_executable(MyApp ${APP_SOURCES})
# Set the output directory for the MyApp target
set_target_properties(MyApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

# Call add third-party sources to output
add_third_party_sources(MyApp)

# Setup 3rd party dlls and library links
setup_links_and_DLLs(MyApp)
