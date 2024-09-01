#pragma once
// Windows specific for sockets
#include <winsock2.h>

#include <cstdint>
#include <vector>

#include "InterfaceDefinition/BaseClass.h"

class SocketStreamInterface : public ByteStreamBase {
 public:
  SocketStreamInterface();
  ~SocketStreamInterface();

  // Reads data from the UDP socket and returns it as a vector of bytes
  std::vector<uint8_t> readData();

 private:
  SOCKET udpSocket;  // The UDP socket

  // Initializes the UDP socket
  void initSocket();
};
