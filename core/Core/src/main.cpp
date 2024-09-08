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

int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});

  uint8_t i = 0;
  uint8_t x = 0;

  StyleManager styler;
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow",
      std::bind(&StyleManager::showStyleAdjustmentWindow, &styler));

  // Main loop to render the GUI
  while (GuiInterface::getInstance().render()) {
  }

  return 0;
}
