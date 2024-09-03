#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include "ByteStream/ByteStreamInterface.h"
#include "Display/GuiInterface.h"
// Define and register a new ImGui window for dynamic content
void customwindow() {
  ImGui::Begin("Dynamic Window");

  ImGui::Text("This is a dynamically added window.");
  if (ImGui::Button("Press Me")) {
    std::cout << "Dynamic button pressed!" << std::endl;
  }

  ImGui::End();
};
int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});

  // Initialize the GUI window
  GuiInterface window;

  // Register the dynamic window with the GUI interface
  window.registerWindow(customwindow);

  // Main loop to render the GUI
  while (window.render()) {
    // Additional application logic or data processing can go here
  }

  return 0;
}
