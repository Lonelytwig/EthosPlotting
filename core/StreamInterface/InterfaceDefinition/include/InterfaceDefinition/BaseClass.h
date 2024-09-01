#pragma once

#include <stdint.h>

#include <string>


class ByteStreamBase {
 public:
  /**
   * @brief Interface function to define handle to read bytes from stream
   * interface
   *
   * @param buffer
   * @param buffer_length
   * @return uint64_t Number of bytes received
   */
  virtual uint64_t get_bytes(uint8_t *buffer, uint64_t buffer_length) = 0;

  /**
   * @brief Interface function to define handle to send bytes to stream
   * interface
   *
   */
  virtual void send_bytes(uint8_t *buffer, uint64_t buffer_length) = 0;
};

// Define a struct to hold configuration parameters
struct UdpConfig {
  std::string recv_ip;  // IP address to receive from
  uint16_t recv_port;   // Port number to receive from
  std::string send_ip;  // IP address to send to
  uint16_t send_port;   // Port number to send to
};
