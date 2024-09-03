#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"


// Function type alias for dynamic window rendering
using RenderCallback = std::function<void()>;

class GuiInterface {
 public:
  GuiInterface();
  ~GuiInterface();

  // Render the GUI, returns false if the window should close
  bool render();

  // Register a new window callback
  void registerWindow(const RenderCallback& callback);

  // Unregister a window callback
  void unregisterWindow(const RenderCallback& callback);

 private:
  // Poll for user input events
  void pollEvents();

  // Setup window with OpenGL and ImGui
  void createWindow(int width, int height, const char* title);

  // Render all registered windows
  void renderWindows();

  // Tear down resources when closing the window
  void closeWindowResources();

  GLFWwindow* window_;
  std::vector<RenderCallback> render_callbacks_;
};
