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
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.1.zip
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

# Incorporate needed 3rd party source files to project executable
function(add_third_party_sources target_name)
    # Append sources to the target
    set(3RD_PARTY_PROJECT_SOURCES ${IMGUI_SOURCES} ${IMPLOT_SOURCES})
    target_sources(${target_name} PRIVATE ${3RD_PARTY_PROJECT_SOURCES})
endfunction()

# Function to set up dependencies for a target
function(setup_links_and_DLLs target_name)
    # Link libraries
    target_link_libraries(${target_name} PRIVATE ${GLEW_LIBRARY} ${GLFW_LIBRARY} OpenGL::GL)

    # Copy GLEW DLL to the output directory after building
    add_custom_command(TARGET ${target_name} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different
                       "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
                       "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endfunction()