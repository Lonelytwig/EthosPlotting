#pragma once

#include <string>

#include "Display/GuiInterface.h"

// Class for creating and managing custom buttons
class CustomButton {
 public:
  CustomButton(const std::string& label);

  // Render the button with current settings
  void render();

  // Set button colors
  void setButtonColor(const ImVec4& normal, const ImVec4& hover,
                      const ImVec4& active);

  // Set button size
  void setButtonSize(const ImVec2& size);

  // Getters for the GUI to adjust button properties
  ImVec4 getNormalColor() const;
  ImVec4 getHoverColor() const;
  ImVec4 getActiveColor() const;
  ImVec2 getSize() const;

 private:
  std::string label_;    // Button label
  ImVec4 color_normal_;  // Normal color
  ImVec4 color_hover_;   // Hover color
  ImVec4 color_active_;  // Active color
  ImVec2 size_;          // Button size
};
