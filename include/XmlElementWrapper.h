#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include "tinyxml2.h"

class XmlElementWrapper
{
public:
    XmlElementWrapper(tinyxml2::XMLElement* element, const char* name, bool createIfNotFound = false);

    tinyxml2::XMLElement* getElement() const;

    // Overloaded operator>> for deserializing
    bool operator>>(std::string& value);
    bool operator>>(int& value);
    bool operator>>(unsigned int& value);
    bool operator>>(int64_t& value);
    bool operator>>(uint64_t& value);
    bool operator>>(bool& value);
    bool operator>>(double& value);
    bool operator>>(float& value);

    // Overloaded operator<< for serializing
    bool operator<<(const std::string& value);
    bool operator<<(int value);
    bool operator<<(unsigned int value);
    bool operator<<(int64_t value);
    bool operator<<(uint64_t value);
    bool operator<<(bool value);
    bool operator<<(double value);
    bool operator<<(float value);

private:
    tinyxml2::XMLElement* element;

    void reportError(const char* value);
    bool getText(std::string& value);
    bool setText(const std::string& value);
    bool setText(const char* value);

    template<typename T>
    bool setText(const T& value)
    {
        if (!element)
        {
            std::cerr << "Error: Element is null." << std::endl;
            return false;
        }
        element->SetText(std::to_string(value).c_str());
        return true;
    }

    template<typename Func, typename T>
    bool queryText(Func func, T& value)
    {
        if (!element)
        {
            std::cerr << "Error: Element is null." << std::endl;
            return false;
        }
        if ((element->*func)(&value) != tinyxml2::XMLError::XML_SUCCESS)
        {
            std::cerr << "Error: Failed to query value from element." << std::endl;
            return false;
        }
        return true;
    }
};

// External helper functions for deserialization
bool deserialize(tinyxml2::XMLElement* element, const char* name, std::string& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, int& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, unsigned int& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, int64_t& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, uint64_t& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, bool& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, double& value);
bool deserialize(tinyxml2::XMLElement* element, const char* name, float& value);

// External helper functions for serialization
bool serialize(tinyxml2::XMLElement* element, const char* name, const std::string& value);
bool serialize(tinyxml2::XMLElement* element, const char* name, int value);
bool serialize(tinyxml2::XMLElement* element, const char* name, unsigned int value);
bool serialize(tinyxml2::XMLElement* element, const char* name, int64_t value);
bool serialize(tinyxml2::XMLElement* element, const char* name, uint64_t value);
bool serialize(tinyxml2::XMLElement* element, const char* name, bool value);
bool serialize(tinyxml2::XMLElement* element, const char* name, double value);
bool serialize(tinyxml2::XMLElement* element, const char* name, float value);