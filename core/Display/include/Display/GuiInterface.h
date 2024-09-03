#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

// Function type alias for dynamic window rendering
using RenderCallback = std::function<void()>;

class GuiInterface {
 public:
  // Get the singleton instance
  static GuiInterface& getInstance();

  // Render the GUI, returns false if the window should close
  bool render();

  // Register a new window callback with a unique name
  void registerWindow(const std::string& name, const RenderCallback& callback);

  // Unregister a window callback by its unique name
  void unregisterWindow(const std::string& name);

 private:
  // Private constructor to prevent direct instantiation
  GuiInterface();
  ~GuiInterface();

  // Delete copy constructor and assignment operator to prevent copies
  GuiInterface(const GuiInterface&) = delete;
  GuiInterface& operator=(const GuiInterface&) = delete;

  // Poll for user input events
  void pollEvents();

  // Setup window with OpenGL and ImGui
  void createWindow(int width, int height, const char* title);

  // Render all registered windows
  void renderWindows();

  // Tear down resources when closing the window
  void closeWindowResources();

  GLFWwindow* window_;
  // Map of window names to callbacks
  std::map<std::string, RenderCallback> render_callbacks_;
};
