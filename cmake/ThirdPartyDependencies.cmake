# Download 3rd party libraries
include(FetchContent)

################### GLEW ###################
FetchContent_Declare(
  glew
  URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
  )
FetchContent_MakeAvailable(glew)
# Set up include and lib paths for GLEW
set(GLEW_INCLUDE_DIR "${glew_SOURCE_DIR}/include")
set(GLEW_LIBRARY "${glew_SOURCE_DIR}/lib/Release/x64/glew32.lib") # Adjust path as necessary
include_directories(${GLEW_INCLUDE_DIR})

################### GLFW ###################
FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN64.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
  )
FetchContent_MakeAvailable(glfw)
# Set up include and lib paths for GLFW
set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
set(GLFW_LIBRARY "${glfw_SOURCE_DIR}/lib-mingw-w64/libglfw3.a") # Adjust path as necessary
include_directories(${GLFW_INCLUDE_DIR})

################### ImPlot #################
FetchContent_Declare(
  implot
  URL https://github.com/epezent/implot/archive/refs/tags/v0.16.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
  )
FetchContent_MakeAvailable(implot)
include_directories(${implot_SOURCE_DIR})
file(GLOB IMPLOT_SOURCES "${implot_SOURCE_DIR}/*.cpp")

################### ImGui ##################
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.1-docking.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
  )
FetchContent_MakeAvailable(imgui)
include_directories(${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
# Specify the source files for ImGui and ImPlot
file(GLOB IMGUI_SOURCES 
"${imgui_SOURCE_DIR}/imgui.cpp"
"${imgui_SOURCE_DIR}/imgui_draw.cpp"
"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
"${imgui_SOURCE_DIR}/imgui_tables.cpp"
"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
"${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Find host system OpenGL package
find_package(OpenGL REQUIRED)

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
    # Append sources to the targetyyyyyyyy
    set(3RD_PARTY_PROJECT_SOURCES ${IMGUI_SOURCES} ${IMPLOT_SOURCES} )
    target_sources(${target_name} PRIVATE ${3RD_PARTY_PROJECT_SOURCES})
    target_include_directories(${target_name} PRIVATE ${PROTOBUF_SOURCE})
endfunction()

# Function to set up dependencies for a target
function(setup_links_and_DLLs target_name)
    # Defer the finding of the protobuf library to build time
    set(PROTOBUF_LIB "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotobuf.a>")
    set(PROTOC_LIB "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/protobuf-build/libprotoc.a>")

    # Link libraries
    target_link_libraries(${target_name} PRIVATE 
      ${GLEW_LIBRARY} 
      ${GLFW_LIBRARY} 
      OpenGL::GL 
      ${PROTOBUF_LIB} ${PROTOC_LIB}
      )

    # Copy GLEW DLL to the output directory after building
    add_custom_command(TARGET ${target_name} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
                       "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endfunction()