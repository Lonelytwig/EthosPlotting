#pragma once
#include <string>

#include "imgui.h"

class StyleManager {
 public:
  /**
   * @brief Called by main render loop to display tabs
   *
   */
  void Render();

 private:
  /**
   * @brief Opens style adjustment window for manually editing of all window
   * color options
   *
   */
  void ShowStyleAdjustmentWindow();

  /**
   * @brief Manages logic to display menu options
   *
   */
  void ShowMainMenuBar();

  /**
   * @brief Custom default modern green style
   *
   */
  void ApplyModernGreenStyle();

  /**
   * @brief Save current style to file
   *
   * @param filename
   */
  void SaveStyleToFile(const std::string& filename);

  /**
   * @brief Load saved style from file
   *
   * @param filename
   */
  void LoadStyleFromFile(const std::string& filename);

  /**
   * @brief Save user custom style to file
   *
   * @param style_name
   */
  void SaveCustomStyle(const std::string& style_name);

  /**
   * @brief Manage GUI style options
   *
   */
  void GeneralStyleAdjustments();

  /**
   * @brief Manage GUI scaler options
   *
   */
  void ShowScalingAdjustmentWindow();

  bool show_style_adjustment_window_ = false;
  bool show_scaling_adjustment_window_ = false;

  const char* save_style_file_name_ = "style.cfg";
};
