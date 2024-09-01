# Set the source files for the static library
set(BYTESTREAM_SRC
    src/SocketStream.cc
)

# Add the static library to the build system
add_library(ByteStreamInterfaces STATIC ${BYTESTREAM_SRC})

# Specify the include directories for the library
target_include_directories(ByteStreamInterfaces PUBLIC include)

