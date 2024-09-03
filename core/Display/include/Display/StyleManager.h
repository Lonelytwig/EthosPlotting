#pragma once
#include <string>
#include <vector>

#include "imgui.h"


class StyleManager {
 public:
  // Constructor: Load the default style
  StyleManager();

  // Show the style adjustment window
  void showStyleAdjustmentWindow();

  // Apply a predefined style preset
  void applyPreset(const std::string& presetName);

  // Save the current style configuration
  void saveCurrentStyle();

 private:
  ImGuiStyle saved_style_;  // Stores the last configured style

  // Method to apply the default Modern Green style
  void applyModernGreenStyle();

  // Helper function to apply a style
  void applyStyle(const ImGuiStyle& style);
};
