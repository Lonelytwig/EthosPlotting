#include "ByteStream/ByteStreamInterface.h"

ByteStreamInterface::ByteStreamInterface() {
  interface_pointer = std::make_unique<SocketStreamInterface>();
}

uint64_t ByteStreamInterface::get_bytes(uint8_t *buffer,
                                        uint64_t buffer_length) {
  return interface_pointer->get_bytes(buffer, buffer_length);
}

void ByteStreamInterface::send_bytes(uint8_t *buffer, uint64_t buffer_length) {
  interface_pointer->send_bytes(buffer, buffer_length);
}
