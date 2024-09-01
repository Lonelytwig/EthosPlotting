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
  if (bind(udpSocket, (const struct sockaddr*)&serverAddress,
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

std::vector<uint8_t> SocketStreamInterface::readData() {
  std::vector<uint8_t> buffer(BUFFER_SIZE);
  struct sockaddr_in clientAddress;
  int clientLen = sizeof(clientAddress);

  // Receive data from the socket in non-blocking mode
  int n =
      recvfrom(udpSocket, reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE,
               0, (struct sockaddr*)&clientAddress, &clientLen);

  if (n == SOCKET_ERROR) {
    int error = WSAGetLastError();
    if (error == WSAEWOULDBLOCK) {
      // No data available, fall through and return an empty vector
      return {};
    } else {
      std::cerr << "Failed to receive data. Error: " << error << std::endl;
      return {};
    }
  }

  buffer.resize(
      n);  // Resize the buffer to the actual size of the received data
  return buffer;
}
