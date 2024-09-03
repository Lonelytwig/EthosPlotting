#include "Display/StyleManager.h"

// Constructor: Load the default style
StyleManager::StyleManager() {}

// Show the style adjustment window
void StyleManager::showStyleAdjustmentWindow() {
  // Load the default style (Modern Green)
  applyModernGreenStyle();
  // Save the style so we have the initial style state
  saveCurrentStyle();
  ImGuiStyle& style = ImGui::GetStyle();

  // Adjust overall colors
  ImGui::Text("Colors");
  for (int i = 0; i < ImGuiCol_COUNT; i++) {
    const char* name = ImGui::GetStyleColorName(i);
    if (ImGui::ColorEdit4(name, (float*)&style.Colors[i])) {
      saveCurrentStyle();  // Save the style whenever a change is made
    }
  }

  // Adjust frame rounding
  ImGui::Text("Rounding");
  if (ImGui::SliderFloat("Frame Rounding", &style.FrameRounding, 0.0f, 12.0f,
                         "%.0f")) {
    saveCurrentStyle();
  }

  // Adjust other style settings as needed
  ImGui::Text("Padding and Spacing");
  if (ImGui::SliderFloat2("Window Padding", (float*)&style.WindowPadding, 0.0f,
                          20.0f) ||
      ImGui::SliderFloat2("Frame Padding", (float*)&style.FramePadding, 0.0f,
                          20.0f) ||
      ImGui::SliderFloat2("Item Spacing", (float*)&style.ItemSpacing, 0.0f,
                          20.0f)) {
    saveCurrentStyle();
  }

  // Preset options
  if (ImGui::Button("Apply Modern Green Preset")) {
    applyPreset("Modern Green");
  }
}

// Apply a predefined style preset
void StyleManager::applyPreset(const std::string& presetName) {
  if (presetName == "Modern Green") {
    applyModernGreenStyle();
    saveCurrentStyle();  // Save after applying the preset
  }
}

// Save the current style configuration
void StyleManager::saveCurrentStyle() {
  saved_style_ = ImGui::GetStyle();  // Save the current ImGui style
}

// Apply the default Modern Green style
void StyleManager::applyModernGreenStyle() {
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
  colors[ImGuiCol_WindowBg] =
      ImVec4(0.10f, 0.15f, 0.12f, 1.00f);  // Darker green background
  colors[ImGuiCol_ChildBg] = ImVec4(
      0.12f, 0.17f, 0.14f, 1.00f);  // Slightly lighter green for child windows
  colors[ImGuiCol_FrameBg] =
      ImVec4(0.16f, 0.22f, 0.18f, 1.00f);  // Mid-green for frames
  colors[ImGuiCol_FrameBgHovered] = ImVec4(
      0.20f, 0.28f, 0.22f, 0.80f);  // Hovered frames with a brighter green
  colors[ImGuiCol_FrameBgActive] =
      ImVec4(0.24f, 0.33f, 0.27f, 1.00f);  // Active frames
  colors[ImGuiCol_Button] =
      ImVec4(0.14f, 0.21f, 0.17f, 1.00f);  // Button base color
  colors[ImGuiCol_ButtonHovered] =
      ImVec4(0.18f, 0.26f, 0.21f, 1.00f);  // Button hover color
  colors[ImGuiCol_ButtonActive] =
      ImVec4(0.22f, 0.30f, 0.25f, 1.00f);  // Button active color
  colors[ImGuiCol_Header] = ImVec4(0.18f, 0.28f, 0.22f, 1.00f);  // Header color
  colors[ImGuiCol_HeaderHovered] =
      ImVec4(0.22f, 0.32f, 0.26f, 0.80f);  // Header hovered color
  colors[ImGuiCol_HeaderActive] =
      ImVec4(0.24f, 0.36f, 0.30f, 1.00f);  // Header active color
  colors[ImGuiCol_ScrollbarBg] =
      ImVec4(0.10f, 0.15f, 0.12f, 1.00f);  // Scrollbar background
  colors[ImGuiCol_ScrollbarGrab] =
      ImVec4(0.24f, 0.34f, 0.30f, 1.00f);  // Scrollbar grab
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.26f, 0.36f, 0.32f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.28f, 0.39f, 0.35f, 1.00f);
  colors[ImGuiCol_TitleBg] =
      ImVec4(0.10f, 0.15f, 0.12f, 1.00f);  // Title background
  colors[ImGuiCol_TitleBgActive] =
      ImVec4(0.22f, 0.32f, 0.27f, 1.00f);  // Title active background
  colors[ImGuiCol_Text] =
      ImVec4(0.88f, 0.93f, 0.88f, 1.00f);  // Light text for contrast
  colors[ImGuiCol_Border] =
      ImVec4(0.20f, 0.28f, 0.23f, 0.60f);  // Subtle borders
  colors[ImGuiCol_PopupBg] =
      ImVec4(0.12f, 0.16f, 0.14f, 0.95f);  // Pop-up background

  // Remove excessive borders for a cleaner look
  style.WindowBorderSize = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.PopupBorderSize = 1.0f;
}

// Helper function to apply a style
void StyleManager::applyStyle(const ImGuiStyle& style) {
  ImGui::GetStyle() = style;  // Apply the given style
}
