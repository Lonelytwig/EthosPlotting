#include "Display/StyleManager.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::map<std::string, ImGuiStyle> saved_styles_;  // To store saved styles

void StyleManager::ShowMainMenuBar() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Style")) {
      GeneralStyleAdjustments();
      if (ImGui::MenuItem("Adjust Scaling")) {
        show_scaling_adjustment_window_ = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void StyleManager::GeneralStyleAdjustments() {
  if (ImGui::BeginMenu("Change Style")) {
    if (ImGui::MenuItem("Custom")) {
      show_style_adjustment_window_ = true;
    }

    // Dynamically load saved styles as menu items
    for (const auto& [name, style] : saved_styles_) {
      if (ImGui::MenuItem(name.c_str())) {
        ImGui::GetStyle() = style;
      }
    }

    // Default Style Presets
    if (ImGui::MenuItem("Classic")) {
      ImGui::StyleColorsClassic();
      SaveStyleToFile(save_style_file_name_);
    }
    if (ImGui::MenuItem("Dark")) {
      ImGui::StyleColorsDark();
      SaveStyleToFile(save_style_file_name_);
    }
    if (ImGui::MenuItem("Light")) {
      ImGui::StyleColorsLight();
      SaveStyleToFile(save_style_file_name_);
    }
    // Custom Style Presets
    if (ImGui::MenuItem("Modern Green")) {
      ApplyModernGreenStyle();
      SaveStyleToFile(save_style_file_name_);
    }

    ImGui::EndMenu();
  }
}

void StyleManager::ShowStyleAdjustmentWindow() {
  if (show_style_adjustment_window_) {
    ImGui::Begin("Style Adjustment", &show_style_adjustment_window_);
    ImGuiStyle& style = ImGui::GetStyle();

    // Adjust overall colors
    ImGui::Text("Colors");
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
      const char* name = ImGui::GetStyleColorName(i);
      ImGui::ColorEdit4(name, (float*)&style.Colors[i]);
    }

    // Adjust frame rounding
    ImGui::Text("Rounding");
    ImGui::SliderFloat("Frame Rounding", &style.FrameRounding, 0.0f, 12.0f,
                       "%.0f");

    // Adjust other style settings as needed
    ImGui::Text("Padding and Spacing");
    ImGui::SliderFloat2("Window Padding", (float*)&style.WindowPadding, 0.0f,
                        20.0f);
    ImGui::SliderFloat2("Frame Padding", (float*)&style.FramePadding, 0.0f,
                        20.0f);
    ImGui::SliderFloat2("Item Spacing", (float*)&style.ItemSpacing, 0.0f,
                        20.0f);

    // Apply or Reset Buttons
    if (ImGui::Button("Apply")) {
      SaveStyleToFile(save_style_file_name_);
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
      ImGui::StyleColorsDark();  // Reset to default style
      SaveStyleToFile(save_style_file_name_);
    }

    // Save custom style button
    static char style_name[128] = "";
    ImGui::InputText("Style Name", style_name, IM_ARRAYSIZE(style_name));
    // Save the current style with the given name
    if (ImGui::Button("Save Custom Style")) {
      SaveCustomStyle(style_name);
    }

    ImGui::End();
  }
}

void StyleManager::ShowScalingAdjustmentWindow() {
  if (show_scaling_adjustment_window_) {
    ImGui::Begin("Scaling Adjustment", &show_scaling_adjustment_window_);
    ImGuiIO& io = ImGui::GetIO();

    // Adjust font size
    ImGui::Text("Text Size");
    // Slider for text size
    ImGui::SliderFloat("Font Scale", &io.FontGlobalScale, 0.5f, 2.0f, "%.1f");

    ImGui::End();
  }
}

void StyleManager::SaveCustomStyle(const std::string& style_name) {
  if (style_name.empty()) return;  // Do not save if the style name is empty

  ImGuiStyle& current_style = ImGui::GetStyle();
  saved_styles_[style_name] = current_style;

  // Append the new style to the file
  std::ofstream ofs(save_style_file_name_, std::ios::app | std::ios::binary);
  if (!ofs) {
    std::cerr << "Failed to open file for writing: " << save_style_file_name_
              << std::endl;
    return;
  }

  size_t name_length = style_name.size();
  ofs.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
  ofs.write(style_name.c_str(), name_length);
  ofs.write(reinterpret_cast<const char*>(&current_style), sizeof(ImGuiStyle));
  ofs.write(reinterpret_cast<const char*>(current_style.Colors),
            sizeof(ImVec4) * ImGuiCol_COUNT);
  ofs.close();
}

void StyleManager::LoadStyleFromFile(const std::string& filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs) {
    std::cerr << "No style configuration file found, loading default."
              << std::endl;
    return;
  }

  saved_styles_.clear();  // Clear any existing styles

  while (ifs.peek() != EOF) {
    size_t name_length;
    if (!ifs.read(reinterpret_cast<char*>(&name_length), sizeof(name_length)))
      break;
    if (name_length == 0 || name_length > 128) {  // Check for valid name length
      std::cerr << "Invalid style name length encountered: " << name_length
                << std::endl;
      break;
    }

    std::string style_name(name_length, '\0');
    if (!ifs.read(&style_name[0], name_length)) break;

    ImGuiStyle style;
    if (!ifs.read(reinterpret_cast<char*>(&style), sizeof(ImGuiStyle))) break;
    if (!ifs.read(reinterpret_cast<char*>(style.Colors),
                  sizeof(ImVec4) * ImGuiCol_COUNT))
      break;

    saved_styles_[style_name] = style;  // Load the style into memory
  }

  ifs.close();
}

void StyleManager::SaveStyleToFile(const std::string& filename) {
  std::ofstream ofs(filename, std::ios::out | std::ios::binary);
  if (!ofs) {
    std::cerr << "Failed to open file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& [name, style] : saved_styles_) {
    size_t name_length = name.size();
    ofs.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
    ofs.write(name.c_str(), name_length);
    ofs.write(reinterpret_cast<const char*>(&style), sizeof(ImGuiStyle));
    ofs.write(reinterpret_cast<const char*>(style.Colors),
              sizeof(ImVec4) * ImGuiCol_COUNT);
  }

  ofs.close();
}

void StyleManager::Render() {
  static bool startup = true;
  if (startup) {
    LoadStyleFromFile(save_style_file_name_);
    startup = false;
  }
  ShowMainMenuBar();
  ShowStyleAdjustmentWindow();
  ShowScalingAdjustmentWindow();
}

void StyleManager::ApplyModernGreenStyle() {
  ImGuiStyle& style = ImGui::GetStyle();

  // Rounded corners and padding for a modern feel
  style.WindowRounding = 7.0f;
  style.FrameRounding = 7.0f;
  style.GrabRounding = 7.0f;
  style.ScrollbarRounding = 12.0f;
  style.ChildRounding = 7.0f;
  style.FramePadding = ImVec2(12, 6);
  style.ItemSpacing = ImVec2(12, 8);

  // High contrast and smooth colors for a modern forestry green look
  ImVec4* colors = style.Colors;
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.15f, 0.12f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.17f, 0.14f, 1.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.22f, 0.18f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.28f, 0.22f, 0.80f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.33f, 0.27f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.14f, 0.21f, 0.17f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.26f, 0.21f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.30f, 0.25f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.18f, 0.28f, 0.22f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.32f, 0.26f, 0.80f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.36f, 0.30f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.15f, 0.12f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.34f, 0.30f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.36f, 0.32f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.28f, 0.39f, 0.35f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.15f, 0.12f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.32f, 0.27f, 1.00f);
  colors[ImGuiCol_Text] = ImVec4(0.88f, 0.93f, 0.88f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.20f, 0.28f, 0.23f, 0.60f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.16f, 0.14f, 0.95f);

  // Tabs styling for a green look
  colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.21f, 0.17f, 1.00f);  // Tab base color
  colors[ImGuiCol_TabHovered] =
      ImVec4(0.18f, 0.26f, 0.21f, 1.00f);  // Tab hovered
  colors[ImGuiCol_TabActive] =
      ImVec4(0.22f, 0.30f, 0.25f, 1.00f);  // Tab active
  colors[ImGuiCol_TabUnfocused] =
      ImVec4(0.10f, 0.15f, 0.12f, 1.00f);  // Unfocused tab
  colors[ImGuiCol_TabUnfocusedActive] =
      ImVec4(0.14f, 0.21f, 0.17f, 1.00f);  // Active but unfocused

  colors[ImGuiCol_MenuBarBg] = ImVec4(
      0.14f, 0.21f, 0.17f, 1.00f);  // Green color for menu bar background

  // Remove excessive borders for a cleaner look
  style.WindowBorderSize = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.PopupBorderSize = 1.0f;
}
