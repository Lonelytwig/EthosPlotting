#include "SocketStream/UdpStream.h"

#include <winsock2.h>  // Include Winsock2 header for SOCKET functions
#include <ws2tcpip.h>  // Include for additional Winsock functions

#include <cstring>
#include <iostream>

// Constructor that accepts UdpConfig struct
UdpStreamInterface::UdpStreamInterface(const UdpConfig &config)
    : config_(config) {
  initSocket();  // Initialize the UDP socket
}

UdpStreamInterface::~UdpStreamInterface() {
  closesocket(udpSocket_recv);  // Close the receive socket
  closesocket(udpSocket_send);  // Close the send socket
  WSACleanup();                 // Clean up Winsock resources
}

void UdpStreamInterface::initSocket() {
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    std::cerr << "WSAStartup failed with error: " << result << std::endl;
    exit(EXIT_FAILURE);
  }

  // Setup receiving socket
  udpSocket_recv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (udpSocket_recv == INVALID_SOCKET) {
    std::cerr << "Failed to create receive socket. Error: " << WSAGetLastError()
              << std::endl;
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  u_long mode = 1;  // Enable non-blocking mode
  if (ioctlsocket(udpSocket_recv, FIONBIO, &mode) != 0) {
    std::cerr << "Failed to set non-blocking mode. Error: " << WSAGetLastError()
              << std::endl;
    closesocket(udpSocket_recv);
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in recvAddress;
  memset(&recvAddress, 0, sizeof(recvAddress));
  recvAddress.sin_family = AF_INET;
  recvAddress.sin_addr.s_addr =
      inet_addr(config_.recv_ip.c_str());  // Use configured receive IP
  recvAddress.sin_port =
      htons(config_.recv_port);  // Use configured receive port

  if (bind(udpSocket_recv, (const struct sockaddr *)&recvAddress,
           sizeof(recvAddress)) == SOCKET_ERROR) {
    std::cerr << "Failed to bind receive socket. Error: " << WSAGetLastError()
              << std::endl;
    closesocket(udpSocket_recv);
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  std::cout << "Receive socket initialized on " << config_.recv_ip << ":"
            << config_.recv_port << std::endl;

  // Setup sending socket
  udpSocket_send = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (udpSocket_send == INVALID_SOCKET) {
    std::cerr << "Failed to create sending socket. Error: " << WSAGetLastError()
              << std::endl;
    return;
  }

  // No need to bind the sending socket as it only needs to send data
}

uint64_t UdpStreamInterface::get_bytes(uint8_t *buffer,
                                       uint64_t buffer_length) {
  struct sockaddr_in clientAddress;
  int clientLen = sizeof(clientAddress);

  int64_t num_bytes =
      recvfrom(udpSocket_recv, reinterpret_cast<char *>(buffer), buffer_length,
               0, (struct sockaddr *)&clientAddress, &clientLen);

  if (num_bytes == SOCKET_ERROR) {
    int error = WSAGetLastError();
    if (error == WSAEWOULDBLOCK) {
      return 0;  // No data available
    } else if (error == WSAEMSGSIZE) {
      std::cerr << "Buffer too small for incoming message. Error: " << error
                << std::endl;
      return 0;
    } else {
      std::cerr << "Failed to receive data. Error: " << error << std::endl;
      return 0;
    }
  }

  return static_cast<uint64_t>(num_bytes);
}

void UdpStreamInterface::send_bytes(uint8_t *buffer, uint64_t buffer_length) {
  struct sockaddr_in destAddress;
  memset(&destAddress, 0, sizeof(destAddress));
  destAddress.sin_family = AF_INET;
  destAddress.sin_addr.s_addr =
      inet_addr(config_.send_ip.c_str());           // Use configured send IP
  destAddress.sin_port = htons(config_.send_port);  // Use configured send port

  int64_t bytes_sent = sendto(
      udpSocket_send, reinterpret_cast<const char *>(buffer), buffer_length, 0,
      (struct sockaddr *)&destAddress, sizeof(destAddress));

  if (bytes_sent == SOCKET_ERROR) {
    std::cerr << "Failed to send data. Error: " << WSAGetLastError()
              << std::endl;
  } else {
    std::cout << "Sent " << bytes_sent << " bytes to " << config_.send_ip << ":"
              << config_.send_port << std::endl;
  }
}
