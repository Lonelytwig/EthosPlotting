#pragma once
#include <string>

#include "imgui.h"

class StyleManager {
 public:
  void render();

 private:
  ImGuiStyle saved_style_;  // Stores the last configured style

  void showStyleAdjustmentWindow();
  // Show the style adjustment window
  void showMainMenuBar();

  void applyModernGreenStyle();

  void saveStyleToFile(const std::string& filename);
  void loadStyleFromFile(const std::string& filename);

  bool show_style_adjustment_window_ = false;

  const char* save_style_file_name_ = "style.cfg";
};
