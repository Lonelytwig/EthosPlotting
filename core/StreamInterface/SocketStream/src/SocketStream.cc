#include "SocketStream/SocketStream.h"

#include <winsock2.h>  // Include Winsock2 header for SOCKET functions
#include <ws2tcpip.h>  // Include for additional Winsock functions

#include <cstring>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")  // Link against Winsock library

#define PORT 8080         // The port number to listen on
#define BUFFER_SIZE 1024  // Size of the buffer for incoming data

SocketStreamInterface::SocketStreamInterface() {
  initSocket();  // Initialize the UDP socket
}

SocketStreamInterface::~SocketStreamInterface() {
  closesocket(udpSocket);  // Close the UDP socket
  WSACleanup();            // Clean up Winsock resources
}

void SocketStreamInterface::initSocket() {
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    std::cerr << "WSAStartup failed with error: " << result << std::endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverAddress;

  // Create the UDP socket
  udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (udpSocket == INVALID_SOCKET) {
    std::cerr << "Failed to create socket. Error: " << WSAGetLastError()
              << std::endl;
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  // Set the socket to non-blocking mode
  u_long mode = 1;  // 1 to enable non-blocking mode
  if (ioctlsocket(udpSocket, FIONBIO, &mode) != 0) {
    std::cerr << "Failed to set non-blocking mode. Error: " << WSAGetLastError()
              << std::endl;
    closesocket(udpSocket);
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  // Zero out the server address structure
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;  // Use IPv4
  serverAddress.sin_addr.s_addr =
      INADDR_ANY;  // Listen on all network interfaces
  serverAddress.sin_port =
      htons(PORT);  // Convert port number to network byte order

  // Bind the socket to the specified port and address
  if (bind(udpSocket, (const struct sockaddr *)&serverAddress,
           sizeof(serverAddress)) == SOCKET_ERROR) {
    std::cerr << "Failed to bind socket. Error: " << WSAGetLastError()
              << std::endl;
    closesocket(udpSocket);
    WSACleanup();
    exit(EXIT_FAILURE);
  }

  std::cout
      << "Socket initialized, set to non-blocking mode, and bound to port "
      << PORT << std::endl;
}

uint64_t SocketStreamInterface::get_bytes(uint8_t *buffer,
                                          uint64_t buffer_length) {
  struct sockaddr_in clientAddress;
  int clientLen = sizeof(clientAddress);

  // Attempt to receive data from the socket in non-blocking mode
  int64_t num_bytes =
      recvfrom(udpSocket, reinterpret_cast<char *>(buffer), buffer_length, 0,
               (struct sockaddr *)&clientAddress, &clientLen);

  if (num_bytes == SOCKET_ERROR) {
    int error = WSAGetLastError();
    if (error == WSAEWOULDBLOCK) {
      // No data available at the moment; return 0 bytes
      return 0;
    } else if (error == WSAEMSGSIZE) {
      // Buffer was too small
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

void SocketStreamInterface::send_bytes(uint8_t *buffer,
                                       uint64_t buffer_length) {}
