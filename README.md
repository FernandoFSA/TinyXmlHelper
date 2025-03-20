# TinyXmlHelper
A lightweight C++ library for simplifying XML serialization and deserialization using TinyXML2.

## Overview
TinyXmlHelper provides a wrapper around TinyXML2 to make it easier to read from and write to XML files. It includes:
- `XmlElementWrapper`: A class to handle individual XML elements with intuitive `>>` (deserialize) and `<<` (serialize) operators.
- `XMLSerializable`: A base class for objects that can be serialized to or deserialized from XML.

This library is perfect for beginners and developers who want a simple way to work with XML in C++.

## Features
- Supports common data types: `std::string`, `int`, `float`, `double`, `bool`, etc.
- Automatic creation of XML elements during serialization.
- Error handling with detailed console output.
- Flexible and extensible with the `XMLSerializable` class.

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building with CMake](#building-with-cmake)
- [Environment Setup](#environment-setup)
- [Example Usage](#example-usage)
- [Real-World Example](#real-world-example)
- [Contributing](#contributing)
- [License](#license)
- [Getting Help](#getting-help)
- [Contributors](#contributors)

## Prerequisites
- C++11 or later
- [TinyXML2](https://github.com/leethomason/tinyxml2) library

## Installation
### 1. Clone the Repository
```bash
git clone https://github.com/FernandoFSA/TinyXmlHelper.git
```

### 2. Install TinyXML2
Download TinyXML2 from [TinyXML2](https://github.com/leethomason/tinyxml2) and compile it.

#### On Linux:
```bash
sudo apt-get install libtinyxml2-dev
```

#### On Windows:
1. Download TinyXML2 from GitHub.
2. Build it with CMake.
3. Link `tinyxml2.lib` (or `tinyxml2.a` for static linking) to your project.

### 3. Add TinyXmlHelper to Your Project
Copy the `include/` and `src/` folders into your project. Then, include the headers in your code:
```cpp
#include "XmlElementWrapper.h"
#include "XMLSerializable.h"
```

## Building with CMake
TinyXmlHelper includes a `CMakeLists.txt` for easy building:

1. **Install Prerequisites**:
   - Ensure CMake is installed (`sudo apt-get install cmake` on Ubuntu).
   - Install TinyXML2 (see [Environment Setup](#environment-setup)).

2. **Build the Project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the Example**:
   ```bash
   ./bin/simple_example  # On Linux/Mac
   bin\simple_example.exe  # On Windows
   ```

4. **Integrate into Your Project**:
   - Add TinyXmlHelper as a subdirectory in your CMake project:
     ```cmake
     add_subdirectory(TinyXmlHelper)
     target_link_libraries(your_target PRIVATE TinyXmlHelper)
     ```

## Environment Setup
To use TinyXmlHelper, you’ll need:
1. **A C++ Compiler**: GCC, Clang, or MSVC (C++11 or later)
2. **TinyXML2** (see installation steps above)
3. **Build Your Project**:
   Example with g++:
   ```bash
   g++ -o myprogram main.cpp src/XmlElementWrapper.cpp src/XMLSerializable.cpp -ltinyxml2
   ```

## Example Usage
Here's a quick example of how to use `XmlElementWrapper` to serialize an integer:

```cpp
#include "XmlElementWrapper.h"
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("root");
    doc.LinkEndChild(root);

    int value = 42;
    XmlElementWrapper wrapper(root);
    wrapper << value;

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    std::cout << printer.CStr() << std::endl;

    return 0;
}
```

## Real-World Example
Here's a more complex example of serializing and deserializing a custom object:

```cpp
#include "XmlElementWrapper.h"
#include "XMLSerializable.h"
#include <iostream>

class Person : public XMLSerializable {
public:
    std::string name;
    int age;

    void Serialize(XmlElementWrapper& wrapper) const override {
        wrapper << name << age;
    }

    void Deserialize(XmlElementWrapper& wrapper) override {
        wrapper >> name >> age;
    }
};

int main() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("people");
    doc.LinkEndChild(root);

    Person person;
    person.name = "John Doe";
    person.age = 30;

    XmlElementWrapper wrapper(root);
    wrapper << person;

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    std::cout << printer.CStr() << std::endl;

    Person deserializedPerson;
    wrapper >> deserializedPerson;
    std::cout << "Name: " << deserializedPerson.name << ", Age: " << deserializedPerson.age << std::endl;

    return 0;
}
```

## Complex Examples
Two complex examples are provided:
- `complex_example.cpp`
- `complex_example2.cpp`

These examples demonstrate more advanced usage of the library including handling complex nested objects and reading from an XML file.

### Running Complex Example 1
To compile and run `complex_example.cpp`, use the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/complex_example  # On Linux/Mac
bin\complex_example.exe  # On Windows
```

### Running Complex Example 2
`complex_example2.cpp` requires an input XML file `resources/complex_example2_input.xml`. Make sure the file is available in the `resources` directory.
To compile and run `complex_example2.cpp`, use the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/complex_example2  # On Linux/Mac
bin\complex_example2.exe  # On Windows
```

## Contributing
Feel free to submit issues or pull requests! This project is open to improvements and feedback.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Getting Help
If you need help, you can:
- Check the [documentation](https://github.com/FernandoFSA/TinyXmlHelper/wiki).
- Browse the [issues](https://github.com/FernandoFSA/TinyXmlHelper/issues) for similar questions.
- Open a new issue with details about your problem.

## Contributors
Thanks to the following people who have contributed to this project:
- [FernandoFSA](https://github.com/FernandoFSA)
- [Your Name Here](https://github.com/yourusername)