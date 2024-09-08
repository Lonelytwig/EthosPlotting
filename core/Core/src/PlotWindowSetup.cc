#include "Core/PlotWindowSetup.h"

#include <algorithm>
#include <cmath>  // For sin()

// Function to simulate available serial ports (replace with actual
// implementation)
std::vector<std::string> GetAvailableSerialPorts() {
  return {"COM1", "COM2", "COM3"};  // Example ports, replace with actual list
}

// Constructor for ByteStreamConfig
ByteStreamConfig::ByteStreamConfig()
    : selected_stream_type(0),
      port(8080),
      baud_rate(9600),
      selected_serial_port(0) {
  strcpy(ip_address, "192.168.1.1");
}
// Custom button rendering function with hover animation using public ImGui API
bool CustomHoverButton(const char* label,
                       const ImVec2& size_arg = ImVec2(0, 0)) {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

  // Calculate button size and position
  ImVec2 size =
      ImVec2(size_arg.x > 0.0f ? size_arg.x
                               : label_size.x + style.FramePadding.x * 2.0f,
             size_arg.y > 0.0f ? size_arg.y
                               : label_size.y + style.FramePadding.y * 2.0f);
  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImVec2 min_pos = pos;
  ImVec2 max_pos = ImVec2(pos.x + size.x, pos.y + size.y);

  // Create an invisible button
  ImGui::InvisibleButton(label, size);

  // Check if the button is hovered or clicked
  bool hovered = ImGui::IsItemHovered();
  bool clicked = ImGui::IsItemClicked();

  // Draw button text
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddText(ImVec2(min_pos.x + style.FramePadding.x,
                            min_pos.y + style.FramePadding.y),
                     ImGui::GetColorU32(ImGuiCol_Text), label);

  // Draw animated lines on hover
  if (hovered) {
    float animation_time = static_cast<float>(ImGui::GetTime());
    float max_width = max_pos.x - min_pos.x;
    float top_line_width =
        max_width *
        (0.5f + 0.5f * std::sin(animation_time * 3.0f));  // Right to left
    float bottom_line_width =
        max_width *
        (0.5f + 0.5f * std::sin(animation_time * 3.0f));  // Left to right

    draw_list->AddLine(ImVec2(max_pos.x, min_pos.y),
                       ImVec2(max_pos.x - top_line_width, min_pos.y),
                       ImGui::GetColorU32(ImGuiCol_ButtonHovered),
                       2.0f);  // Top line
    draw_list->AddLine(ImVec2(min_pos.x, max_pos.y),
                       ImVec2(min_pos.x + bottom_line_width, max_pos.y),
                       ImGui::GetColorU32(ImGuiCol_ButtonHovered),
                       2.0f);  // Bottom line
  }

  return clicked;
}

void PlotWindowSetup::Render() {
  static std::vector<ByteStreamConfig> byte_streams;
  static int new_stream_index = 0;

  // Set up resizable columns
  ImGui::Columns(2, "ByteStreamColumns", true);
  ImGui::SetColumnWidth(0, 250.0f);  // Set initial width for the first column

  for (size_t i = 0; i < byte_streams.size(); ++i) {
    auto& config = byte_streams[i];

    // Left column: Byte Stream Select and Configuration
    ImGui::Text("Byte Stream Select");
    const char* stream_types[] = {"UDP Socket", "Serial Port"};
    ImGui::Combo(("##StreamType" + std::to_string(i)).c_str(),
                 &config.selected_stream_type, stream_types,
                 IM_ARRAYSIZE(stream_types));

    if (config.selected_stream_type == 0) {  // UDP Socket selected
      ImGui::InputText(("IP Address##" + std::to_string(i)).c_str(),
                       config.ip_address, IM_ARRAYSIZE(config.ip_address));
      ImGui::InputInt(("Port##" + std::to_string(i)).c_str(), &config.port);
    } else if (config.selected_stream_type == 1) {  // Serial Port selected
      ImGui::InputInt(("Baud Rate##" + std::to_string(i)).c_str(),
                      &config.baud_rate);
      std::vector<std::string> serial_ports = GetAvailableSerialPorts();
      ImGui::Combo(("Available Ports##" + std::to_string(i)).c_str(),
                   &config.selected_serial_port,
                   [](void* data, int idx, const char** out_text) {
                     auto& ports =
                         *static_cast<std::vector<std::string>*>(data);
                     *out_text = ports[idx].c_str();
                     return true;
                   },
                   static_cast<void*>(&serial_ports), serial_ports.size());
    }

    // Move to the right column for message listeners
    ImGui::NextColumn();

    // Right column: Message Listener Configuration
    ImGui::Text("Message Listeners");

    for (size_t j = 0; j < config.message_listeners.size(); ++j) {
      ImGui::Text("Message Listener %d", j + 1);
      ImGui::Indent();
      ImGui::Text("Framer Select");
      const char* framer_options[] = {"COBS", "SLIP",
                                      "Standard Header + Footer"};
      int selected_framer = 0;
      ImGui::Combo(
          ("##FramerSelect" + std::to_string(i) + std::to_string(j)).c_str(),
          &selected_framer, framer_options, IM_ARRAYSIZE(framer_options));

      ImGui::Text("Validate CRC");
      const char* crc_types[] = {"CRC32", "CRC16", "CRC8"};
      int selected_crc_type = 0;
      ImGui::Combo(
          ("##CRCType" + std::to_string(i) + std::to_string(j)).c_str(),
          &selected_crc_type, crc_types, IM_ARRAYSIZE(crc_types));

      static int polynomial = 0;
      ImGui::InputInt(
          ("Polynomial##" + std::to_string(i) + std::to_string(j)).c_str(),
          &polynomial);

      ImGui::Text("Message Type");
      const char* message_types[] = {"Custom Data Structure", "Proto File",
                                     "C Header File"};
      int selected_message_type = 0;
      ImGui::Combo(
          ("##MessageType" + std::to_string(i) + std::to_string(j)).c_str(),
          &selected_message_type, message_types, IM_ARRAYSIZE(message_types));

      if (CustomHoverButton(("Message Field Tagging##" + std::to_string(i) +
                             std::to_string(j))
                                .c_str())) {
        ImGui::OpenPopup(("Message Field Tagging Popup##" + std::to_string(i) +
                          std::to_string(j))
                             .c_str());
      }

      if (ImGui::BeginPopupModal(("Message Field Tagging Popup##" +
                                  std::to_string(i) + std::to_string(j))
                                     .c_str())) {
        ImGui::Text("Hello World");
        if (ImGui::Button("Close")) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
      ImGui::Unindent();
    }

    // "Attach New Message Listener" button
    if (CustomHoverButton(
            ("Attach New Message Listener " + std::to_string(i)).c_str())) {
      config.message_listeners.push_back(config.message_listeners.size());
    }

    ImGui::NextColumn();
    ImGui::Separator();
  }

  if (CustomHoverButton("+ Add New Byte Stream")) {
    byte_streams.push_back(ByteStreamConfig());
  }
}

void PlotWindowSetup::renderStructureEditorWindow() {
  static std::string structureName = "MyStructure";
  static std::vector<FieldInfo> fields;
  static FieldInfo newField = {DataType::UINT8, "newField", 1};

  ImGui::Begin("Structure Editor");

  ImGui::InputText("Structure Name", &structureName[0], 64);
  ImGui::Separator();

  if (ImGui::BeginChild("Field Editor", ImVec2(0, 300), true)) {
    for (int i = 0; i < fields.size(); i++) {
      ImGui::PushID(i);

      ImGui::Text("%d: %s, %s, Array size: %d", i + 1, fields[i].name.c_str(),
                  (fields[i].type == DataType::UINT8     ? "uint8_t"
                   : fields[i].type == DataType::UINT16  ? "uint16_t"
                   : fields[i].type == DataType::UINT32  ? "uint32_t"
                   : fields[i].type == DataType::FLOAT32 ? "float32"
                   : fields[i].type == DataType::FLOAT64 ? "float64"
                   : fields[i].type == DataType::CHAR    ? "char"
                                                         : "unknown"),
                  fields[i].arraySize);

      ImGui::SameLine();
      if (ImGui::Button("Delete")) {
        fields.erase(fields.begin() + i);
        i--;
      }
      ImGui::PopID();
    }
    ImGui::EndChild();
  }

  ImGui::Separator();
  ImGui::InputText("Field Name", &newField.name[0], 64);
  ImGui::InputInt("Array Size", &newField.arraySize);
  if (newField.arraySize < 1) newField.arraySize = 1;

  const char* dataTypes[] = {"int8",   "uint8",   "int16",   "uint16", "int32",
                             "uint32", "float32", "float64", "char"};
  int currentType = static_cast<int>(newField.type);
  ImGui::Combo("Data Type", &currentType, dataTypes, IM_ARRAYSIZE(dataTypes));
  newField.type = static_cast<DataType>(currentType);

  if (ImGui::Button("Add Field")) {
    if (std::find_if(fields.begin(), fields.end(), [&](const FieldInfo& field) {
          return field.name == newField.name;
        }) == fields.end()) {
      fields.push_back(newField);
      newField.name = "newField";
      newField.arraySize = 1;
    } else {
      ImGui::OpenPopup("Error: Duplicate Field Name");
    }
  }

  if (ImGui::BeginPopup("Error: Duplicate Field Name")) {
    ImGui::Text("A field with this name already exists.");
    if (ImGui::Button("OK")) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  ImGui::End();

  messageDefinition.structureName = structureName;
  messageDefinition.fields = fields;
}

MessageDefinition PlotWindowSetup::getMessageDefinition() const {
  return messageDefinition;
}
