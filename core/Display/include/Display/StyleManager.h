#pragma once
#include <string>

#include "imgui.h"

class StyleManager {
 public:
  /**
   * @brief Called by main render loop to display tabs
   *
   */
  void render();

 private:
  /**
   * @brief Opens style adjustment window for manually editing of all window
   * color options
   *
   */
  void showStyleAdjustmentWindow();

  /**
   * @brief Manages logic to display menu options
   *
   */
  void showMainMenuBar();

  /**
   * @brief Custom default modern green style
   *
   */
  void applyModernGreenStyle();

  /**
   * @brief Save current style to file
   *
   * @param filename
   */
  void saveStyleToFile(const std::string& filename);

  /**
   * @brief Load saved style from file
   *
   * @param filename
   */
  void loadStyleFromFile(const std::string& filename);

  void applyStyle(const ImGuiStyle& style);

  void saveCustomStyle(const std::string& style_name);

  bool show_style_adjustment_window_ = false;

  const char* save_style_file_name_ = "style.cfg";
};
