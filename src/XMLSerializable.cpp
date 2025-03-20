#include "XMLSerializable.h"

bool XMLSerializable::serialize(tinyxml2::XMLElement* element) const
{
    bool allSuccess = true;
    for (const auto& data : m_data)
    {
        XmlElementWrapper wrapper(element, data.first.c_str(), true);  // For serialization
        bool success = false;
        if (data.second.type() == typeid(std::string))
            success = wrapper << std::any_cast<std::string>(data.second);
        else if (data.second.type() == typeid(int))
            success = wrapper << std::any_cast<int>(data.second);
        else if (data.second.type() == typeid(unsigned int))
            success = wrapper << std::any_cast<unsigned int>(data.second);
        else if (data.second.type() == typeid(int64_t))
            success = wrapper << std::any_cast<int64_t>(data.second);
        else if (data.second.type() == typeid(uint64_t))
            success = wrapper << std::any_cast<uint64_t>(data.second);
        else if (data.second.type() == typeid(bool))
            success = wrapper << std::any_cast<bool>(data.second);
        else if (data.second.type() == typeid(double))
            success = wrapper << std::any_cast<double>(data.second);
        else if (data.second.type() == typeid(float))
            success = wrapper << std::any_cast<float>(data.second);

        if (!success)
        {
            onSerializeError(data.first, "Failed to write value to element");
            allSuccess = false;
        }
    }
    return allSuccess;
}

bool XMLSerializable::deserialize(tinyxml2::XMLElement* element)
{
    bool allSuccess = true;
    for (auto& [key, value] : m_data)
    {
        XmlElementWrapper wrapper(element, key.c_str(), false);  // For deserialization
        bool success = false;
        if (value.type() == typeid(std::string))
        {
            std::string temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(int))
        {
            int temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(unsigned int))
        {
            unsigned int temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(int64_t))
        {
            int64_t temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(uint64_t))
        {
            uint64_t temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(bool))
        {
            bool temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(double))
        {
            double temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        else if (value.type() == typeid(float))
        {
            float temp;
            success = wrapper >> temp;
            if (success) value = temp;
            else onDeserializeError(key, "Element missing or invalid");
        }
        allSuccess &= success;
    }
    return allSuccess;
}

XmlElementWrapper& operator<<(XmlElementWrapper& wrapper, const XMLSerializable& serializable)
{
    serializable.serialize(wrapper.getElement());
    return wrapper;
}

XmlElementWrapper& operator>>(XmlElementWrapper& wrapper, XMLSerializable& serializable)
{
    serializable.deserialize(wrapper.getElement());
    return wrapper;
}