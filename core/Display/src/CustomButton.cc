#include "Display/CustomButton.h"

#include <iostream>

CustomButton::CustomButton(const std::string& label)
    : label_(label),
      color_normal_(ImVec4(0.26f, 0.59f, 0.98f, 1.00f)),  // Default blue color
      color_hover_(ImVec4(0.40f, 0.70f, 1.00f, 1.00f)),   // Default hover color
      color_active_(
          ImVec4(0.23f, 0.46f, 0.84f, 1.00f)),  // Default active color
      size_(ImVec2(100, 30))                    // Default button size
{}

// Render the button with the current settings
void CustomButton::render() {
  // Set colors for the button
  //   ImGui::PushStyleColor(ImGuiCol_Button, color_normal_);
  //   ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover_);
  //   ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active_);

  // Render the button and check for clicks
  bool clicked = ImGui::Button(label_.c_str(), size_);

  // Pop the color settings back to default
  //   ImGui::PopStyleColor(3);
  if (clicked) {
    std::cout << "Custom button clicked!" << std::endl;
  }
}

void CustomButton::setButtonColor(const ImVec4& normal, const ImVec4& hover,
                                  const ImVec4& active) {
  color_normal_ = normal;
  color_hover_ = hover;
  color_active_ = active;
}

void CustomButton::setButtonSize(const ImVec2& size) { size_ = size; }

ImVec4 CustomButton::getNormalColor() const { return color_normal_; }
ImVec4 CustomButton::getHoverColor() const { return color_hover_; }
ImVec4 CustomButton::getActiveColor() const { return color_active_; }
ImVec2 CustomButton::getSize() const { return size_; }
