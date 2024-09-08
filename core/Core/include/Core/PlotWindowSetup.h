#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <cstring>
#include <string>
#include <vector>

#include "Display/GuiInterface.h"

enum class DataType {
  INT8,
  UINT8,
  INT16,
  UINT16,
  INT32,
  UINT32,
  FLOAT32,
  FLOAT64,
  CHAR
};

// Structure to hold information about fields in a message definition
struct FieldInfo {
  DataType type;
  std::string name;
  int arraySize;

  FieldInfo(DataType t, const std::string& n, int size)
      : type(t), name(n), arraySize(size) {}
};

// Structure to hold a message definition
struct MessageDefinition {
  std::string structureName;
  std::vector<FieldInfo> fields;
};

// Structure to hold the configuration for a byte stream
struct ByteStreamConfig {
  int selected_stream_type;
  char ip_address[16];
  int port;
  int baud_rate;
  int selected_serial_port;
  std::vector<int> message_listeners;

  ByteStreamConfig();
};

// GUI Interface class
class PlotWindowSetup {
 public:
  void Render();
  void renderStructureEditorWindow();
  MessageDefinition getMessageDefinition() const;

 private:
  MessageDefinition messageDefinition;
};

// Function to simulate available serial ports (replace with actual
// implementation)
std::vector<std::string> GetAvailableSerialPorts();

#endif  // GUI_INTERFACE_H
