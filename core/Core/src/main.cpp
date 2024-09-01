#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include "ByteStream/ByteStreamInterface.h"
#include "Display/GuiInterface.h"

// Error callback function for GLFW
static void glfw_error_callback(int error, const char* description) {
  std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int, char**) {
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});
  GuiInterface window;

  while (true) {
    window.render();
  }

  return 0;
}
