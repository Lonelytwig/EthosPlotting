#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include <chrono>
#include <thread>

#include "ByteStream/ByteStreamInterface.h"
#include "Display/CustomButton.h"
#include "Display/GuiInterface.h"
#include "Display/StyleManager.h"

int z = 0;  // Dynamic window counter
CustomButton button("WOWAS");
// Adjustments window callback function for the button
void adjustmentsWindow() {
  // Color adjustment
  ImVec4 color_normal = button.getNormalColor();
  ImVec4 color_hover = button.getHoverColor();
  ImVec4 color_active = button.getActiveColor();

  if (ImGui::ColorEdit4("Normal Color", (float*)&color_normal)) {
    button.setButtonColor(color_normal, color_hover, color_active);
  }
  if (ImGui::ColorEdit4("Hover Color", (float*)&color_hover)) {
    button.setButtonColor(color_normal, color_hover, color_active);
  }
  if (ImGui::ColorEdit4("Active Color", (float*)&color_active)) {
    button.setButtonColor(color_normal, color_hover, color_active);
  }

  // Size adjustment
  ImVec2 size = button.getSize();
  if (ImGui::SliderFloat2("Button Size", (float*)&size, 50.0f, 200.0f)) {
    button.setButtonSize(size);
  }
}

// Define and register a new ImGui window for dynamic content
void customWindow(int id) {
  std::string buttonName = "This is Dynamic Button " + std::to_string(id);
  ImGui::Text(buttonName.c_str());
  if (ImGui::Button(buttonName.c_str())) {
    std::cout << buttonName << " pressed!" << std::endl;
  }
}

int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});

  uint8_t i = 0;
  uint8_t x = 0;

  // GuiInterface::getInstance().registerWindow("adjustmentsWindow",
  //                                            adjustmentsWindow);
  StyleManager styler;
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow",
      std::bind(&StyleManager::showStyleAdjustmentWindow, &styler));
  GuiInterface::getInstance().registerWindow(
      "button to render", std::bind(&CustomButton::render, &button));

  bool buttons_visible = false;
  const uint8_t cycle_limit = 50;  // Number of iterations per cycle
  uint8_t cycle_count =
      0;  // Counter to control appearance/disappearance cycles

  // Main loop to render the GUI
  while (GuiInterface::getInstance().render()) {
    // Manage cyclic appearance/disappearance of buttons
    if (cycle_count == 0 && !buttons_visible) {
      // Register the buttons
      for (int id = 0; id < 5; ++id) {
        std::string button_name = "Dynamic Button " + std::to_string(id);
        GuiInterface::getInstance().registerWindow(
            button_name, [id]() { customWindow(id); });
      }
      buttons_visible = true;
    } else if (cycle_count == cycle_limit && buttons_visible) {
      // Unregister the buttons
      for (int id = 0; id < 5; ++id) {
        std::string button_name = "Dynamic Button " + std::to_string(id);
        GuiInterface::getInstance().unregisterWindow(button_name);
      }
      buttons_visible = false;
      cycle_count = 0;  // Reset cycle count to start again
    }

    // Increment the cycle count
    ++cycle_count;

    // Simulate a delay for the condition
    ++i;
  }

  return 0;
}
