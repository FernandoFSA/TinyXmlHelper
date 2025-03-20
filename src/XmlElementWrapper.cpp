#include "XmlElementWrapper.h"

XmlElementWrapper::XmlElementWrapper(tinyxml2::XMLElement* element, const char* name, bool createIfNotFound)
{
    if (!element)
    {
        std::cerr << "Error: Parent element is null for '" << name << "'" << std::endl;
        this->element = nullptr;
        return;
    }

    this->element = element->FirstChildElement(name);
    if (!this->element && createIfNotFound)
    {
        this->element = element->GetDocument()->NewElement(name);
        element->InsertEndChild(this->element);
    }
    else if (!this->element)
    {
        std::cerr << "Error: Element with name '" << name << "' not found under '"
            << element->Name() << "'" << std::endl;
    }
}

tinyxml2::XMLElement* XmlElementWrapper::getElement() const { return element; }

// Overloaded operator>> for deserializing
bool XmlElementWrapper::operator>>(std::string& value) { return getText(value); }
bool XmlElementWrapper::operator>>(int& value) { return queryText(&tinyxml2::XMLElement::QueryIntText, value); }
bool XmlElementWrapper::operator>>(unsigned int& value) { return queryText(&tinyxml2::XMLElement::QueryUnsignedText, value); }
bool XmlElementWrapper::operator>>(int64_t& value) { return queryText(&tinyxml2::XMLElement::QueryInt64Text, value); }
bool XmlElementWrapper::operator>>(uint64_t& value) { return queryText(&tinyxml2::XMLElement::QueryUnsigned64Text, value); }
bool XmlElementWrapper::operator>>(bool& value) { return queryText(&tinyxml2::XMLElement::QueryBoolText, value); }
bool XmlElementWrapper::operator>>(double& value) { return queryText(&tinyxml2::XMLElement::QueryDoubleText, value); }
bool XmlElementWrapper::operator>>(float& value) { return queryText(&tinyxml2::XMLElement::QueryFloatText, value); }

// Overloaded operator<< for serializing
bool XmlElementWrapper::operator<<(const std::string& value) { return setText(value); }
bool XmlElementWrapper::operator<<(int value) { return setText(value); }
bool XmlElementWrapper::operator<<(unsigned int value) { return setText(value); }
bool XmlElementWrapper::operator<<(int64_t value) { return setText(value); }
bool XmlElementWrapper::operator<<(uint64_t value) { return setText(value); }
bool XmlElementWrapper::operator<<(bool value) { return setText(value ? "true" : "false"); }
bool XmlElementWrapper::operator<<(double value) { return setText(value); }
bool XmlElementWrapper::operator<<(float value) { return setText(value); }

void XmlElementWrapper::reportError(const char* value)
{
    std::cerr << "Error: Element with name '" << value << "' not found." << std::endl;
    std::exit(EXIT_FAILURE);
}

bool XmlElementWrapper::getText(std::string& value)
{
    if (!element)
    {
        std::cerr << "Error: Element is null." << std::endl;
        return false;
    }
    const char* text = element->GetText();
    if (!text)
    {
        std::cerr << "Error: Element text is null." << std::endl;
        return false;
    }
    value = text;
    return true;
}

// Specialize for std::string
bool XmlElementWrapper::setText(const std::string& value)
{
    if (!element)
    {
        std::cerr << "Error: Element is null." << std::endl;
        return false;
    }
    element->SetText(value.c_str());
    return true;
}

// Specialize for const char*
bool XmlElementWrapper::setText(const char* value)
{
    if (!element)
    {
        std::cerr << "Error: Element is null." << std::endl;
        return false;
    }
    element->SetText(value);
    return true;
}

// External helper functions for deserialization
bool deserialize(tinyxml2::XMLElement* element, const char* name, std::string& value)
{
    XmlElementWrapper wrapper(element, name, false);  // For deserialization
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, int& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, unsigned int& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, int64_t& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, uint64_t& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, bool& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, double& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

bool deserialize(tinyxml2::XMLElement* element, const char* name, float& value)
{
    XmlElementWrapper wrapper(element, name, false);
    return wrapper >> value;
}

// External helper functions for serialization
bool serialize(tinyxml2::XMLElement* element, const char* name, const std::string& value)
{
    XmlElementWrapper wrapper(element, name, true);  // For serialization
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, int value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, unsigned int value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, int64_t value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, uint64_t value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, bool value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, double value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}

bool serialize(tinyxml2::XMLElement* element, const char* name, float value)
{
    XmlElementWrapper wrapper(element, name, true);
    return wrapper << value;
}