#pragma once
// Windows specific for sockets
#include <winsock2.h>

#include <cstdint>
#include <vector>

#include "InterfaceDefinition/BaseClass.h"
/**
 * @note Consider using a vector to store frames larger than the input buffer so
 * the caller can piece meal the byte stream out?
 *
 */
class SocketStreamInterface : public ByteStreamBase {
 public:
  SocketStreamInterface();
  ~SocketStreamInterface();

  uint64_t get_bytes(uint8_t *buffer, uint64_t buffer_length) final;

  void send_bytes(uint8_t *buffer, uint64_t buffer_length) final;

 private:
  SOCKET udpSocket;  // The UDP socket

  // Initializes the UDP socket
  void initSocket();
};
