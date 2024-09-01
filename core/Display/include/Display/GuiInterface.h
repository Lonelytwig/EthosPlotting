#pragma once
// Window manager
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>  // Only necessary if you're using native functions

class GuiInterface {
 public:
  GuiInterface();
  ~GuiInterface();

  /**
   * @brief Display application logical layout
   *
   */
  bool render();

 private:
  /**
   * @brief Listen for user inputs
   *
   */
  void pollEvents();

  void renderStructureEditorWindow();
  /**
   * @brief Tear down gui window
   *
   */
  void CloseWindowResources();

  GLFWwindow* window_;
};
