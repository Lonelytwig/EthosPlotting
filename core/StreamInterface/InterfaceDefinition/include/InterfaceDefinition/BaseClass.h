#pragma once

#include <stdint.h>

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
