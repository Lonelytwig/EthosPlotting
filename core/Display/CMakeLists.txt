# Add the static library to the build system
add_library(GuiDisplay STATIC
    src/MainWindow.cc)

# Specify the include directories for the library
target_include_directories(GuiDisplay PUBLIC include)
# Link against implemented stream interfaces
# target_link_libraries(GuiDisplay PUBLIC
#     ByteStreamInterfaceDefintion
#     SocketStreamInterface)
