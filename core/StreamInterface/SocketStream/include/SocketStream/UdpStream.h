#pragma once
// Windows specific for sockets
#include <winsock2.h>

#include <cstdint>

#include "InterfaceDefinition/BaseClass.h"

/**
 * @note Consider using a vector to store frames larger than the input buffer so
 * the caller can piece meal the byte stream out?
 *
 */
class UdpStreamInterface : public ByteStreamBase {
 public:
  // Constructor that takes UdpConfig struct as parameter
  UdpStreamInterface(const UdpConfig &config);
  ~UdpStreamInterface();

  uint64_t get_bytes(uint8_t *buffer, uint64_t buffer_length) final;
  void send_bytes(uint8_t *buffer, uint64_t buffer_length) final;

 private:
  SOCKET udpSocket_recv;  // Inbound traffic
  SOCKET udpSocket_send;  // Outbound traffic

  UdpConfig config_;  // Store the configuration

  // Initializes the UDP socket
  void initSocket();
};
