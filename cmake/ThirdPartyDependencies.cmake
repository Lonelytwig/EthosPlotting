################### Protobuf ###############
include(ExternalProject)

# Set up protobuf building as an external project with custom steps
ExternalProject_Add(protobuf
    # Clone protobuf repository
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG main

    # Initialize and update submodules
    UPDATE_COMMAND git submodule update --init

    # Configure download, build and output locations
    PREFIX "${CMAKE_BINARY_DIR}/_deps/protobuf-src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/protobuf-build"
    # Run CMake
    CMAKE_ARGS
        -G "MinGW Makefiles"
        -DCMAKE_BUILD_TYPE=Release
        -Dprotobuf_BUILD_TESTS=OFF
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

    # Build the project (no explicit command needed)

    # Install command, do nothing
    INSTALL_COMMAND ""
    # Don't print everything to terminal, push it to a file instead
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)
set(PROTOBUF_LIB_DIR "${CMAKE_BINARY_DIR}/_deps/protobuf-build")
set(PROTOBUF_SOURCE "${CMAKE_BINARY_DIR}/protobuf-src/src/protobuf/src/*")

################### Functions ##############
# Make sure other projects are built before main application
function(build_before_app target_name)
    add_dependencies(${target_name} protobuf)
endfunction(build_before_app)

# Incorporate needed 3rd party source files to project executable
function(add_third_party_sources target_name)
    target_include_directories(${target_name} PRIVATE ${PROTOBUF_SOURCE})
endfunction()

# Function to set up dependencies for a target
function(setup_links_and_DLLs target_name)
    # Defer the finding of the protobuf library to build time
    set(PROTOBUF_LIB "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotobuf.a>")
    set(PROTOC_LIB "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotoc.a>")

    # Link libraries
    target_link_libraries(${target_name} PRIVATE
      ${PROTOBUF_LIB} ${PROTOC_LIB}
      )
endfunction()
