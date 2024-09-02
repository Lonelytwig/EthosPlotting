#include <cmath>
#include <iostream>
#include <vector>

// Project includes
#include "ByteStream/ByteStreamInterface.h"
#include "Display/GuiInterface.h"

int main(int, char**) {
  ByteStreamInterface sock_test(UdpConfig{.recv_ip = "127.0.0.1",
                                          .recv_port = 8080,
                                          .send_ip = "127.0.0.1",
                                          .send_port = 8081});
  GuiInterface window;

  while (window.render()) {
  }

  return 0;
}
