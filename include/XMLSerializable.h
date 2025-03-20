#pragma once
#include <any>
#include <map>
#include <string>
#include "tinyxml2.h"
#include "XmlElementWrapper.h"

class XMLSerializable
{
public:
    virtual bool serialize(tinyxml2::XMLElement* element) const;
    virtual bool deserialize(tinyxml2::XMLElement* element);

    template <typename T>
    void addData(const std::string& key, const T& value)
    {
        m_data[key] = value;
    }

    template <typename T>
    T getData(const std::string& key) const
    {
        return std::any_cast<T>(m_data.at(key));
    }

    template <typename T>
    T getData(const std::string& key, const T& defaultValue) const
    {
        try
        {
            return std::any_cast<T>(m_data.at(key));
        }
        catch (const std::bad_any_cast&)
        {
            return defaultValue;
        }
    }

protected:
    std::map<std::string, std::any> m_data;

    virtual void onDeserializeError(const std::string& key, const std::string& reason) const
    {
        std::cerr << "Deserialize error for '" << key << "': " << reason << " (using default value)" << std::endl;
    }

    virtual void onSerializeError(const std::string& key, const std::string& reason) const
    {
        std::cerr << "Serialize error for '" << key << "': " << reason << std::endl;
    }
};

XmlElementWrapper& operator<<(XmlElementWrapper& wrapper, const XMLSerializable& serializable);
XmlElementWrapper& operator>>(XmlElementWrapper& wrapper, XMLSerializable& serializable);