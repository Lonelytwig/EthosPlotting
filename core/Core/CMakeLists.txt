# Create main application
project(MainApplication)

# Generates .d output (dependency of source file)
add_compile_options(-MMD) # Use -MD if you want to see the compiler includes as well
# Observe all compiler actions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the source files for the executable
set(CORE_SRC
    src/main.cpp          # Main entry point
)

# Add executable using the source files directly
add_executable(MainApplication ${CORE_SRC})

# Add include directory
target_include_directories(MainApplication PUBLIC inc)

# Set the output directories for the executable
set_target_properties(MainApplication PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Set up third-party dependencies
setup_third_party_dependencies(MainApplication)

# Link project static libraries to the executable
target_link_libraries(MainApplication PRIVATE ByteStreamInterface GuiDisplay)
