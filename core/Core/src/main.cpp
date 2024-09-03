#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include <chrono>
#include <thread>

#include "ByteStream/ByteStreamInterface.h"
#include "Display/GuiInterface.h"

int main(int, char**) {
  // Initialize ByteStreamInterface with socket configuration
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});

  uint8_t i = 0;
  uint8_t x = 0;
  int z = 0;
  std::vector<RenderCallback> callbacks;  // Store callbacks to unregister later
  // Main loop to render the GUI
  while (GuiInterface::getInstance().render()) {
    if (i < 10) {
      // Create a unique function object for each window registration
      RenderCallback callback = [z = z++] {
        std::string name("Dynamic Window " + std::to_string(z));
        ImGui::Begin(name.c_str());
        ImGui::Text("This is a dynamically added window.");
        if (ImGui::Button("Press Me")) {
          std::cout << "Dynamic button pressed!" << std::endl;
        }
        ImGui::End();
      };

      // Register the dynamic window with the GUI interface
      GuiInterface::getInstance().registerWindow(callback);
      callbacks.push_back(
          callback);  // Keep track of the callback for later removal
      i++;
    } else if (i >= 10 && x < 10) {
      // Unregister the previously stored callback
      GuiInterface::getInstance().unregisterWindow(callbacks[x]);
      x++;
      if (x == 10) {
        i = 0;
        x = 0;
        callbacks.clear();  // Clear the callback list once all have been
                            // unregistered
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
