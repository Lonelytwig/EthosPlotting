# Add the static library to the build system
add_library(ByteStreamInterfaceDefintion INTERFACE)

# Specify the include directories for the library
target_include_directories(ByteStreamInterfaceDefintion INTERFACE include)
