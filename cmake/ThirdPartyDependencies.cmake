# Include necessary modules
include(ExternalProject)

################### Protobuf Setup ###################
# Set up protobuf building as an external project with custom steps
ExternalProject_Add(protobuf_project
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG main

    # Initialize and update submodules
    UPDATE_COMMAND git submodule update --init

    # Configure download, build, and output locations
    PREFIX "${CMAKE_BINARY_DIR}/_deps/protobuf-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/protobuf-build"
    CMAKE_ARGS
        -G "MinGW Makefiles"
        -DCMAKE_BUILD_TYPE=Release
        -Dprotobuf_BUILD_TESTS=OFF
        -DBUILD_SHARED_LIBS=OFF # Ensure it builds as a static library
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

    # No explicit install command needed; we handle linking ourselves
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

# Define imported targets for the Protobuf libraries
add_library(protobuf STATIC IMPORTED GLOBAL)
set_target_properties(protobuf PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotobuf.a")

add_library(protoc STATIC IMPORTED GLOBAL)
set_target_properties(protoc PROPERTIES
    IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotoc.a")

# Ensure protobuf targets are built before using them
add_dependencies(protobuf protobuf_project)
add_dependencies(protoc protobuf_project)

################### Functions ###################

# Function to set up dependencies for a target
function(setup_third_party_dependencies target_name)
    # Ensure protobuf is built before the target
    add_dependencies(${target_name} protobuf_project)

    # Add include directories for Protobuf
    target_include_directories(${target_name} PRIVATE
        "${CMAKE_BINARY_DIR}/_deps/protobuf-src/src"
    )

    # Link Protobuf libraries
    target_link_libraries(${target_name} PRIVATE
        protobuf
        protoc
    )
endfunction()
