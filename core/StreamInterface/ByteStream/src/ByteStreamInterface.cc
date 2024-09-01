#include "ByteStream/ByteStreamInterface.h"

#include "SocketStream/UdpStream.h"

ByteStreamInterface::ByteStreamInterface() {
  interface_pointer = std::make_unique<UdpStreamInterface>();
}

uint64_t ByteStreamInterface::get_bytes(uint8_t *buffer,
                                        uint64_t buffer_length) {
  return interface_pointer->get_bytes(buffer, buffer_length);
}

void ByteStreamInterface::send_bytes(uint8_t *buffer, uint64_t buffer_length) {
  interface_pointer->send_bytes(buffer, buffer_length);
}
