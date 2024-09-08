#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <map>
#include <string>

#include "Display/StyleManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"

// Function type alias for dynamic window rendering
using RenderCallback = std::function<void()>;

class GuiInterface {
 public:
  /**
   * @brief Enforce there is a single gui window in the enviornment
   *
   * @return GuiInterface&
   */
  static GuiInterface& getInstance();

  /**
   * @brief GUI render loop
   *
   * @return true
   * @return false
   */
  bool render();

  /**
   * @brief Register a new window callback with a unique name
   *
   * @param name
   * @param callback
   */
  void registerWindow(const std::string& name, const RenderCallback& callback);

  /**
   * @brief Unregister a window callback by its unique name
   *
   * @param name
   */
  void unregisterWindow(const std::string& name);

 private:
  GuiInterface();
  ~GuiInterface();

  GuiInterface(const GuiInterface&) = delete;
  GuiInterface& operator=(const GuiInterface&) = delete;

  /**
   * @brief Poll for user input events
   *
   */
  void pollEvents();

  /**
   * @brief Setup window with OpenGL and ImGui
   *
   * @param width
   * @param height
   * @param title
   */
  void createWindow(int width, int height, const char* title);

  /**
   * @brief Render all registered windows
   *
   */
  void renderWindows();

  /**
   * @brief Render all registered menu bars
   *
   */
  void renderMenuBars();

  /**
   * @brief Tear down resources when closing the window
   *
   */
  void closeWindowResources();

  GLFWwindow* window_;
  StyleManager styler_;

  // Map of window names to callbacks
  std::map<std::string, RenderCallback> render_callbacks_;
};
