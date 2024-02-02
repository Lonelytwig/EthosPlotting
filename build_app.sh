#!/bin/bash

# Extract shell script location
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILD_DIR="${SCRIPT_DIR}/build"
CMAKE_DIR="${SCRIPT_DIR}"
OS_PLAT=""

detect_os() {
  case "$OSTYPE" in
    linux*)   OS_PLAT="Linux" ;;
    darwin*)  OS_PLAT="Apple" ;;
    msys*)    OS_PLAT="Windows" ;;
    cygwin*)  OS_PLAT="Windows" ;;
    *)        OS_PLAT="Unknown: $OSTYPE" ;;
  esac

  if [ "$OS_PLAT" = "Unknown: $OSTYPE" ]; then
    case "$(uname -s)" in
      Linux*)     OS_PLAT="Linux" ;;
      Darwin*)    OS_PLAT="Apple" ;;
      CYGWIN*)    OS_PLAT="Windows" ;;
      MINGW*)     OS_PLAT="Windows" ;;
      MSYS*)      OS_PLAT="Windows" ;;
      *)          OS_PLAT="Unknown: $(uname -s)" ;;
    esac
  fi

  echo Detected $OS_PLAT operating system
}

detect_os
# Help prompt
help() {
    echo Build options
    echo
    echo Options
    echo h      Display available options and arguments
    echo b      Build FW binaries
    echo c      Clean build environment
    echo u      Upload binary to device
    echo g      Display dependency graph
    echo
}

# Setup build path and files
setup_build() {
    mkdir "${BUILD_DIR}"
    echo
    (cmake -S "${CMAKE_DIR}" -B "${BUILD_DIR}" -G "MinGW Makefiles" -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
}

# Check for presence of build directory
check_for_build() {
    if [[ ! -d "${BUILD_DIR}" ]]
    then
        echo Build configuration not found
        return 1
    else
        echo Build configuration found
        return 0
    fi
}

# Build application
build() {
    # Verify that the build folder is present
    check_for_build
    if [ "$?" -eq 1 ]; then
        echo Generating new build configuration
        setup_build
    fi
    # Call make on new build with all cpu cores
    echo
    echo Building project
    NPROC=32
    (cd "${BUILD_DIR}" && make -j$NPROC)
}

# Remove build artifacts
clean() {
    echo "Removing ${BUILD_DIR}"
    rm -rf "${BUILD_DIR}"
}

# Upload ouput binary to device
upload() {
    echo NOT IMPLEMENTED
}

# Display build dependency graph
graph() {
    check_for_build
    if [ "$?" -eq 1 ];
    then
        echo Cannot generate dependency graph
        exit
    fi
    echo Generating dependency graph

    DEP_PATH=dependencies

    (cd "${BUILD_DIR}" && \
    cmake --graphviz=dependency_graph.dot . && \
    mkdir -p "${DEP_PATH}" && \
    for i in `find . -name "*.dot*"`; do dot -Tsvg $i > "${DEP_PATH}"/${i#./dependency_graph.dot.}.svg; done && \

    echo
    echo Opening top level dependency graph. For all other graphs navigate to "${BUILD_DIR}/${DEP_PATH}"
    if [ "$OS_PLAT" = "Windows" ]; then
        start "${BUILD_DIR}/${DEP_PATH}/dependency_graph.dot.svg"
    elif [ "$OS_PLAT" = "Linux" ]; then
        eog "${BUILD_DIR}/${DEP_PATH}/dependency_graph.dot.svg"
    fi
    )
}
# Display detailed header dependency graph
generate_header_dependency_graph() {
    # Call make on new build with all cpu cores
    (cmake -S "${CMAKE_DIR}" -B "${BUILD_DIR}" -G "MinGW Makefiles" 2> asd.txt)
    (cd "${BUILD_DIR}" && make -j6 &> asd.txt)
}

# User input
if [ $# -eq 0 ]; then
    >&2 echo "No arguments provided"
    help
    exit 1
fi
while getopts bchguq flag;
do
    case ${flag} in
        b)  # Build program
            build;;
        c)  # Remove build outputs
            clean;;
        h)  # Print out help
            help;;
        g)  # Generate dependency graph
            graph;;
        u)  # Upload FW to device
            upload;;
        q)
            generate_header_dependency_graph;;
    esac
done
