
### Updated `CMakeLists.txt`

```cmake
# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TinyXmlHelper VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the library
add_library(TinyXmlHelper STATIC
    src/XmlElementWrapper.cpp
    src/XMLSerializable.cpp
)

# Specify include directories for the library
target_include_directories(TinyXmlHelper PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Find TinyXML2 (assumes it's installed or available)
find_package(TinyXML2 REQUIRED)
if(TinyXML2_FOUND)
    target_link_libraries(TinyXmlHelper PUBLIC TinyXML2::TinyXML2)
else()
    message(FATAL_ERROR "TinyXML2 not found. Please install it (e.g., 'sudo apt-get install libtinyxml2-dev' on Ubuntu).")
endif()

# Optionally build the example
option(BUILD_EXAMPLES "Build example programs" ON)
if(BUILD_EXAMPLES)
    add_executable(simple_example examples/simple_example.cpp)
    target_link_libraries(simple_example PRIVATE TinyXmlHelper)
    add_executable(complex_example examples/complex_example.cpp)
    target_link_libraries(complex_example PRIVATE TinyXmlHelper)
    add_executable(complex_example2 examples/complex_example2.cpp)
    target_link_libraries(complex_example2 PRIVATE TinyXmlHelper)
endif()

# Installation rules (optional, for users who want to install the library)
install(TARGETS TinyXmlHelper
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include/TinyXmlHelper)

# Set output directories (optional, for cleaner builds)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)