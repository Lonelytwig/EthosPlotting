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

################### ImGui ##################
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.90.1-docking.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(imgui)

# List all ImGui source files explicitly
set(IMGUI_SOURCES
  "${imgui_SOURCE_DIR}/imgui.cpp"
  "${imgui_SOURCE_DIR}/imgui_draw.cpp"
  "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
  "${imgui_SOURCE_DIR}/imgui_tables.cpp"
  "${imgui_SOURCE_DIR}/imgui_demo.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

################### ImPlot #################
FetchContent_Declare(
  implot
  URL https://github.com/epezent/implot/archive/refs/tags/v0.16.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(implot)

# Find all ImPlot source files
file(GLOB IMPLOT_SOURCES "${implot_SOURCE_DIR}/*.cpp")

add_library(implot STATIC ${IMPLOT_SOURCES})
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Find host system OpenGL package
find_package(OpenGL REQUIRED)

################### Display Components Static Library ###################
# Create a static library for display components that aggregates all dependencies
add_library(DisplayComponents STATIC ${IMGUI_SOURCES} ${IMPLOT_SOURCES})

# Add dependencies to the DisplayComponents library
target_include_directories(DisplayComponents PUBLIC
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${implot_SOURCE_DIR}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(DisplayComponents PRIVATE
    ${GLEW_LIBRARY}
    ${GLFW_LIBRARY}
    OpenGL::GL
    imgui
    implot
)

################### GuiDisplay Static Library ###################
# Create the main GuiDisplay library
add_library(GuiDisplay STATIC
  src/MainWindow.cc
  src/CustomButton.cc
  src/ModifyStyle.cc)

# Include necessary directories for GuiDisplay
target_include_directories(GuiDisplay PUBLIC include)

# Link GuiDisplay against DisplayComponents library
target_link_libraries(GuiDisplay PUBLIC DisplayComponents)

#################### Custom Target for Copying DLL ###################
# Create a dummy target that depends on GuiDisplay to perform copy
add_custom_target(copy_glew_dummy ALL DEPENDS GuiDisplay)

# Copy GLEW DLL to the output directory after building GuiDisplay
add_custom_command(TARGET copy_glew_dummy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${glew_SOURCE_DIR}/bin/Release/x64/glew32.dll"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glew32.dll"
    COMMENT "Copying glew32.dll to output directory..."
)
