# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Initial release of TinyXmlHelper

## [1.0.0] - 2025-03-20
### Added
- `XmlElementWrapper` class to handle individual XML elements.
- `XMLSerializable` base class for objects that can be serialized to or deserialized from XML.
- Support for common data types: `std::string`, `int`, `float`, `double`, `bool`, etc.
- Automatic creation of XML elements during serialization.
- Error handling with detailed console output.
- CMake support for building the project.
- Example usage and real-world example in the documentation.

## [1.0.1] - 2025-03-20
### Added
- Two complex examples are provided: `complex_example.cpp`and `complex_example2.cpp`
- descriptions and usage instructions for the new example files added to the [README](README).
- CMakeLists.txt updated to include the new example files in the build process.