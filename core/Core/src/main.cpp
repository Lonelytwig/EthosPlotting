#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>  // Only necessary if you're using native functions

#include <cmath>
#include <iostream>
#include <vector>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

// Project includes
#include "ByteStream/ByteStreamInterface.h"
#include "main_window.h"

// Error callback function for GLFW
static void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char**) {
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  GLFWwindow* window = glfwCreateWindow(
      1280, 720, "Dear ImGui + ImPlot Example with Viewports", NULL, NULL);
  if (window == nullptr) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  // Enable Viewports

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 130";  // GLSL 130 for OpenGL 3.0
  ImGui_ImplOpenGL3_Init(glsl_version);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    // Create an ImGui window
    // Example window flags
    button_test();

    // ImPlot::ShowDemoWindow(nullptr, "a");

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
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

    glfwSwapBuffers(window);

    std::string rx_buffer(128, '\0');
    uint64_t length =
        sock_test.get_bytes(reinterpret_cast<uint8_t*>(rx_buffer.data()), 128);
    if (length) {
      // If needed, resize the string to the actual number of bytes received
      if (length < rx_buffer.size()) {
        rx_buffer.resize(length);
      }

      // Output the received data (assuming it is printable)
      std::cout << "Received data: " << rx_buffer << std::endl;
      sock_test.send_bytes(reinterpret_cast<uint8_t*>(rx_buffer.data()),
                           length);
    }
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
