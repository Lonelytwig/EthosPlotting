# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.20)

project(MyApp)
set(CMAKE_CXX_STANDARD 17)

# Specify module source directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")

# Download 3rd party libraries
include(FetchContent)

# GLEW
FetchContent_Declare(
  glew
  URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip
)
FetchContent_MakeAvailable(glew)

# GLFW
FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN64.zip
)
FetchContent_MakeAvailable(glfw)

# ImPlot
FetchContent_Declare(
  implot
  URL https://github.com/epezent/implot/archive/refs/tags/v0.16.zip
)

# ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.1.zip
)

FetchContent_MakeAvailable(implot imgui)

# Set up include and lib paths for GLEW
set(GLEW_INCLUDE_DIR "${glew_SOURCE_DIR}/include")
set(GLEW_LIBRARY "${glew_SOURCE_DIR}/lib/Release/x64/glew32.lib") # Adjust path as necessary

# Set up include and lib paths for GLFW
set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
set(GLFW_LIBRARY "${glfw_SOURCE_DIR}/lib-mingw-w64/libglfw3.a") # Adjust path as necessary

include_directories(${GLEW_INCLUDE_DIR} ${GLFW_INCLUDE_DIR} ${implot_SOURCE_DIR} ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

# Specify the source files for ImGui and ImPlot
file(GLOB IMGUI_SOURCES 
     "${imgui_SOURCE_DIR}/imgui.cpp"
     "${imgui_SOURCE_DIR}/imgui_draw.cpp"
     "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
     "${imgui_SOURCE_DIR}/imgui_tables.cpp"
     "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
     "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)
file(GLOB IMPLOT_SOURCES "${implot_SOURCE_DIR}/*.cpp")

# Find the OpenGL package
find_package(OpenGL REQUIRED)

# Define the sources for your application
set(APP_SOURCES "${SRC_DIR}/main.cpp" "${imgui_SOURCE_DIR}/imgui_demo.cpp" )


# Add executable
add_executable(MyApp ${APP_SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES})

# Link libraries
target_link_libraries(MyApp PRIVATE ${GLEW_LIBRARY} ${GLFW_LIBRARY} OpenGL::GL)

# Optional: Copy GLEW DLL to the output directory after building
add_custom_command(TARGET MyApp POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
                   $<TARGET_FILE_DIR:MyApp>)
