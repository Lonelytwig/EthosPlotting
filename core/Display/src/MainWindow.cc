#include <iostream>
#include <vector>

#include "Display/GuiInterface.h"

// Error callback function for GLFW
static void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

GuiInterface& GuiInterface::getInstance() {
  static GuiInterface instance;
  return instance;
}

GuiInterface::GuiInterface() {
  // Setup GLFW
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) exit(EXIT_FAILURE);

  createWindow(1280, 720, "Main Window");

  // Initialize Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  const char* glsl_version = "#version 130";
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void GuiInterface::createWindow(int width, int height, const char* title) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window_ = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window_ == nullptr) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err)
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

GuiInterface::~GuiInterface() { closeWindowResources(); }

void GuiInterface::pollEvents() { glfwPollEvents(); }

void GuiInterface::registerWindow(const std::string& name,
                                  const RenderCallback& callback) {
  // Store the callback with its associated name
  render_callbacks_[name] = callback;
}

void GuiInterface::unregisterWindow(const std::string& name) {
  // Remove the callback associated with the given name
  render_callbacks_.erase(name);
}

bool GuiInterface::render() {
  if (glfwWindowShouldClose(window_)) {
    return false;
  }

  glfwMakeContextCurrent(window_);
  pollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  renderWindows();

  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window_, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow* backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }

  glfwSwapBuffers(window_);
  return true;
}

void SetupDocking() {
  // Get the main viewport
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);

  // Create a window that serves as the main dock space
  ImGui::Begin("DockSpace Window", nullptr,
               ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus);

  ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

  // Setup the dock nodes only if not already set
  static bool initialized = false;
  if (!initialized) {
    initialized = true;

    // Remove any existing layout
    ImGui::DockBuilderRemoveNode(dockspace_id);  // Remove existing layout
    ImGui::DockBuilderAddNode(
        dockspace_id, ImGuiDockNodeFlags_DockSpace);  // Create root node
    ImGui::DockBuilderSetNodeSize(dockspace_id,
                                  viewport->WorkSize);  // Set root node size

    // Split the main dockspace into five sub-docks
    ImGuiID dock_id_top =
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr,
                                    &dockspace_id);  // 20% for top window
    ImGuiID dock_id_bottom =
        ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.2f, nullptr,
                                    &dockspace_id);  // 20% for bottom window
    ImGuiID dock_id_center =
        dockspace_id;  // Remaining space in the center (60%)

    // Split the center into two zones: left and right
    ImGuiID dock_id_left_center = ImGui::DockBuilderSplitNode(
        dock_id_center, ImGuiDir_Left, 0.5f, nullptr,
        &dock_id_center);  // 50% left, 50% right
    ImGuiID dock_id_right_center =
        dock_id_center;  // Remaining part on the right

    // Further split the left center dock into three vertical zones
    ImGuiID dock_id_left_top = ImGui::DockBuilderSplitNode(
        dock_id_left_center, ImGuiDir_Up, 0.33f, nullptr, &dock_id_left_center);
    ImGuiID dock_id_left_middle = ImGui::DockBuilderSplitNode(
        dock_id_left_center, ImGuiDir_Up, 0.5f, nullptr, &dock_id_left_center);
    ImGuiID dock_id_left_bottom =
        dock_id_left_center;  // Remaining space goes to bottom

    // Further split the right center dock into two vertical zones
    ImGuiID dock_id_right_top =
        ImGui::DockBuilderSplitNode(dock_id_right_center, ImGuiDir_Up, 0.5f,
                                    nullptr, &dock_id_right_center);
    ImGuiID dock_id_right_bottom =
        dock_id_right_center;  // Remaining space goes to bottom

    // Dock windows into their dedicated docks
    ImGui::DockBuilderDockWindow("Top Window",
                                 dock_id_top);  // Top window in its own dock
    ImGui::DockBuilderDockWindow("Window 1", dock_id_left_top);
    ImGui::DockBuilderDockWindow("Window 2", dock_id_left_middle);
    ImGui::DockBuilderDockWindow("Window 3", dock_id_left_bottom);
    ImGui::DockBuilderDockWindow("Horizontal Window 1", dock_id_right_top);
    ImGui::DockBuilderDockWindow("Horizontal Window 2", dock_id_right_bottom);
    ImGui::DockBuilderDockWindow(
        "Bottom Window", dock_id_bottom);  // Bottom window in its own dock

    // Finalize the dock layout
    ImGui::DockBuilderFinish(dockspace_id);
  }

  ImGui::End();  // End of the main docking space window
}

void GuiInterface::renderWindows() {
  // Render the menu bar at the top
  renderMenuBars();
  SetupDocking();

  // Begin creating the new top window
  ImGui::Begin("Top Window");
  ImGui::Text("This is the Top Window");
  ImGui::End();

  // Begin creating the first window
  ImGui::Begin("Window 1");
  ImGui::Text("This is Window 1");
  ImGui::End();

  // Begin creating the second window
  ImGui::Begin("Window 2");
  ImGui::Text("This is Window 2");
  ImGui::End();

  // Begin creating the third window
  ImGui::Begin("Window 3");
  ImGui::Text("This is Window 3");
  ImGui::End();

  // Begin creating the first horizontal window
  ImGui::Begin("Horizontal Window 1");
  ImGui::Text("This is Horizontal Window 1");
  ImGui::End();

  // Begin creating the second horizontal window
  ImGui::Begin("Horizontal Window 2");
  ImGui::Text("This is Horizontal Window 2");
  ImGui::End();

  // Begin creating the bottom window
  ImGui::Begin("Bottom Window");
  ImGui::Text("This is the Bottom Window");
  ImGui::End();
}

void GuiInterface::renderMenuBars() { styler_.Render(); }

void GuiInterface::closeWindowResources() {
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
