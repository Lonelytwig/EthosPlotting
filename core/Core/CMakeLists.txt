
project(MainApplication)

# Generates .d output (dependency of source file)
add_compile_options(-MMD) # Use -MD if you want to see the compiler includes as well

# Observe all compiler actions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include other cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/ThirdPartyDependencies.cmake)

# Set the source files for the executable
set(CORE_SRC
    src/main.cpp          # Main entry point
    src/main_window.cpp  
)

# Add executable using the source files directly
add_executable(MainApplication ${CORE_SRC})

# Specify the include directories for the executable
target_include_directories(MainApplication PUBLIC inc)

# Define output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../app)

# Optionally, set the output directories for the executable
set_target_properties(MainApplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Build 3rd party projects first
build_before_app(MainApplication)

# Call add third-party sources to output
add_third_party_sources(MainApplication)

# Setup 3rd party dlls and library links
setup_links_and_DLLs(MainApplication)

# Link the ByteStreamInterfaces static library to the executable
target_link_libraries(MainApplication PRIVATE ByteStreamInterfaces)