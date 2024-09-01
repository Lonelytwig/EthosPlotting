# Add the static library to the build system
add_library(ByteStreamInterface STATIC
    src/ByteStreamInterface.cc)

# Specify the include directories for the library
target_include_directories(ByteStreamInterface PUBLIC include)
# Link against implemented stream interfaces
target_link_libraries(ByteStreamInterface PUBLIC
    ByteStreamInterfaceDefintion
    SocketStreamInterface)
