#pragma once

#include <memory>

#include "InterfaceDefinition/BaseClass.h"

class ByteStreamInterface : public ByteStreamBase {
 public:
  ByteStreamInterface();
  ~ByteStreamInterface() = default;

  uint64_t get_bytes(uint8_t *buffer, uint64_t buffer_length) final;

  void send_bytes(uint8_t *buffer, uint64_t buffer_length) final;

 private:
  std::unique_ptr<ByteStreamBase> interface_pointer;
};
