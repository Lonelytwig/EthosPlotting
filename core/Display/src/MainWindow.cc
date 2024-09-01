

#include <cmath>
#include <iostream>
#include <vector>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

// Project includes
#include "Display/GuiInterface.h"

// Error callback function for GLFW
static void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void GuiInterface::pollEvents() { glfwPollEvents(); }

GuiInterface::GuiInterface() {
  // Setup window_
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window_ = glfwCreateWindow(
      1280, 720, "Dear ImGui + ImPlot Example with Viewports", NULL, NULL);
  if (window_ == nullptr) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Viewports

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  const char* glsl_version = "#version 130";  // GLSL 130 for OpenGL 3.0
  ImGui_ImplOpenGL3_Init(glsl_version);
}

bool GuiInterface::render() {
  /* If window should be closed, fall through */
  if (glfwWindowShouldClose(window_)) {
    return false;
  }
  ImGuiIO& io = ImGui::GetIO();

  pollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();
  // Create an ImGui window_
  // Example window_ flags
  // button_test();

  // ImPlot::ShowDemoWindow(nullptr, "a");

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window_, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Update and Render additional Platform Windows
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(window_);

  return true;
}

GuiInterface::~GuiInterface() { CloseWindowResources(); }

void GuiInterface::CloseWindowResources() {
  static bool close_window = false;
  if (!close_window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window_);
    glfwTerminate();

    close_window = true;
  }
}
