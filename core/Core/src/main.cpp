#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include <chrono>
#include <thread>

#include "ByteStream/ByteStreamInterface.h"
#include "Display/CustomButton.h"
#include "Display/GuiInterface.h"

int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});

  // GuiInterface::getInstance().registerWindow(
  //     "styleAdjustmentWindow", std::bind(&StyleManager::render, &styler));

  // Main loop to render the GUI
  while (GuiInterface::getInstance().render()) {
  }

  return 0;
}
