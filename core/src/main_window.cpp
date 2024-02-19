#include <cmath>
#include <iostream>
#include <vector>

// ImGui includes
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
void button_test() {
  ImGuiViewport* main_viewport = ImGui::GetMainViewport();
  float main_menu_bar_height = 0.0f;

  // Main Menu Bar
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Options")) {
      // Button that stays within the main GLFW window
      if (ImGui::MenuItem("Say Hello")) {
        std::cout << "Hello, World!" << std::endl;
      }
      // Button that stays within the main GLFW window
      if (ImGui::MenuItem("Say Hello6")) {
        std::cout << "Hello, World!6" << std::endl;
      }
      // Button that stays within the main GLFW window
      if (ImGui::MenuItem("Say Hello5")) {
        std::cout << "Hello, World!5" << std::endl;
      }
      // Button that stays within the main GLFW window
      if (ImGui::MenuItem("Say Hello3")) {
        std::cout << "Hello, World!4" << std::endl;
      }
      // Button that stays within the main GLFW window
      if (ImGui::MenuItem("Say Hello2")) {
        std::cout << "Hello, World!2" << std::endl;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    main_menu_bar_height = ImGui::GetFrameHeight();
  }

  // Initial position for Custom Window 1
  ImVec2 window1_pos =
      ImVec2(main_viewport->Pos.x, main_viewport->Pos.y + main_menu_bar_height);

  // Begin Custom Window 1 without specifying size to allow resizing
  ImGui::SetNextWindowPos(window1_pos);
  ImGui::Begin("Custom Window 1", nullptr);
  ImGui::Text("Hello from Window 1!");
  ImVec2 window1_size =
      ImGui::GetWindowSize();  // Query the current size after rendering
  ImGui::End();

  // Position for Custom Window 2, immediately below Window 1
  ImVec2 window2_pos = ImVec2(window1_pos.x, window1_pos.y + window1_size.y);

  // Begin Custom Window 2
  ImGui::SetNextWindowPos(window2_pos);
  ImGui::Begin("Custom Window 2", nullptr);
  ImGui::Text("Hello from Window 2!");
  ImVec2 window2_size =
      ImGui::GetWindowSize();  // Query the current size after rendering
  ImGui::End();

  // Calculate position for Custom Window 3 to the right of Window 1 and 2
  // Use the larger height of the two windows if they are not the same
  float max_height = std::max(window1_size.y, window2_size.y);
  ImVec2 window3_pos = ImVec2(window1_pos.x + window1_size.x,
                              main_viewport->Pos.y + main_menu_bar_height);

  // Begin Custom Window 3
  ImGui::SetNextWindowPos(window3_pos);
  ImGui::Begin("Custom Window 3",
               nullptr);  // Allow resizing by not specifying size
  ImGui::Text("Hello from Window 3!");
  ImGui::End();
}