# Add the static library to the build system
add_library(SocketStreamInterface STATIC
    src/SocketStream.cc)

# Specify the include directories for the library
target_include_directories(SocketStreamInterface PUBLIC include)
target_link_libraries(SocketStreamInterface PUBLIC ByteStreamInterfaceDefintion)
