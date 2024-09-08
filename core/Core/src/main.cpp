#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include <chrono>
#include <thread>

#include "ByteStream/ByteStreamInterface.h"
#include "Core/PlotWindowSetup.h"
#include "Display/GuiInterface.h"

int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});
  PlotWindowSetup p1;
  PlotWindowSetup p2;
  PlotWindowSetup p3;
  PlotWindowSetup p4;
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow1", std::bind(&PlotWindowSetup::Render, &p1));
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow2", std::bind(&PlotWindowSetup::Render, &p2));
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow3", std::bind(&PlotWindowSetup::Render, &p3));
  GuiInterface::getInstance().registerWindow(
      "styleAdjustmentWindow4", std::bind(&PlotWindowSetup::Render, &p4));

  // Main loop to render the GUI
  while (GuiInterface::getInstance().render()) {
  }

  return 0;
}
