#include <iostream>
#include <vector>
#include "tinyxml2.h"
#include "XmlElementWrapper.h"
#include "XMLSerializable.h"

// This test program defines a set of classes (`Base`, `Platform`, `LocationSet`, `Imput`) that inherit from the
// `XMLSerializable` base class, enabling XML serialization and deserialization using the TinyXML2 library.
// The `Base` and `Platform` classes represent geographic entities with latitude/longitude coordinates and codes.
// The `LocationSet` class aggregates a `Base` and a vector of `Platforms`. The `Imput` class serves as the top-level
// container for a `LocationSet`. The main functionality includes loading data from an XML file (input.xml), 
// processing it, and saving it to another XML file (output.xml). The program includes error handling for file 
// operations and critical field validation.

// Base class for geographic entity with coordinates
class Base : public XMLSerializable
{
public:
    Base()
    {
        addData("Code", std::string("0"));
        addData("ID", std::string(""));
        addData("LatitudeDegree", 0);
        addData("LatitudeMinute", 0);
        addData("LatitudeSecond", 0.0f);
        addData("LatitudeCP", std::string("N"));
        addData("LongitudeDegree", 0);
        addData("LongitudeMinute", 0);
        addData("LongitudeSecond", 0.0f);
        addData("LongitudeCP", std::string("E"));
    }

    bool serialize(tinyxml2::XMLElement* element) const override { return XMLSerializable::serialize(element); }
    bool deserialize(tinyxml2::XMLElement* element) override { return XMLSerializable::deserialize(element); }

protected:
    void onDeserializeError(const std::string& key, const std::string& reason) const override
    {
        if (key == "Code" || key == "ID")
        {
            throw std::runtime_error("Critical field '" + key + "' missing or invalid in Base");
        }
        else
        {
            std::cerr << "Base: Ignored missing/non-critical field '" << key << "': " << reason << std::endl;
        }
    }
};

// Platform class for additional geographic entities
class Platform : public XMLSerializable
{
public:
    Platform()
    {
        addData("Code", std::string("0"));
        addData("ID", std::string(""));
        addData("LatitudeDegree", 0);
        addData("LatitudeMinute", 0);
        addData("LatitudeSecond", 0.0f);
        addData("LatitudeCP", std::string("N"));
        addData("LongitudeDegree", 0);
        addData("LongitudeMinute", 0);
        addData("LongitudeSecond", 0.0f);
        addData("LongitudeCP", std::string("E"));
    }

    bool serialize(tinyxml2::XMLElement* element) const override { return XMLSerializable::serialize(element); }
    bool deserialize(tinyxml2::XMLElement* element) override { return XMLSerializable::deserialize(element); }
};

// LocationSet class to aggregate Base and Platforms
class LocationSet : public XMLSerializable
{
public:
    LocationSet()
    {
        addData("Base", Base());
        addData("Platforms", std::vector<Platform>());
    }

    bool serialize(tinyxml2::XMLElement* element) const override
    {
        bool allSuccess = true;
        tinyxml2::XMLElement* baseSet = element->GetDocument()->NewElement("BaseSet");
        tinyxml2::XMLElement* baseElement = element->GetDocument()->NewElement("Base");
        Base base = getData<Base>("Base");
        allSuccess &= base.serialize(baseElement);
        baseSet->InsertEndChild(baseElement);
        element->InsertEndChild(baseSet);

        tinyxml2::XMLElement* platformSet = element->GetDocument()->NewElement("PlatformSet");
        std::vector<Platform> platforms = getData<std::vector<Platform>>("Platforms");
        for (const auto& platform : platforms)
        {
            tinyxml2::XMLElement* platformElement = element->GetDocument()->NewElement("Platform");
            allSuccess &= platform.serialize(platformElement);
            platformSet->InsertEndChild(platformElement);
        }
        element->InsertEndChild(platformSet);
        return allSuccess;
    }

    bool deserialize(tinyxml2::XMLElement* element) override
    {
        bool allSuccess = true;
        tinyxml2::XMLElement* baseSet = element->FirstChildElement("BaseSet");
        if (baseSet)
        {
            tinyxml2::XMLElement* baseElement = baseSet->FirstChildElement("Base");
            if (baseElement)
            {
                Base base = getData<Base>("Base");
                allSuccess &= base.deserialize(baseElement);
                addData("Base", base);
            }
        }

        tinyxml2::XMLElement* platformSet = element->FirstChildElement("PlatformSet");
        if (platformSet)
        {
            std::vector<Platform> platforms;
            for (tinyxml2::XMLElement* platformElement = platformSet->FirstChildElement("Platform");
                platformElement != nullptr;
                platformElement = platformElement->NextSiblingElement("Platform"))
            {
                Platform platform = getData<Platform>("Platforms", Platform());
                allSuccess &= platform.deserialize(platformElement);
                platforms.push_back(platform);
            }
            addData("Platforms", platforms);
        }
        return allSuccess;
    }
};

// Top-level Imput class
class Imput : public XMLSerializable
{
public:
    Imput()
    {
        addData("LocationSet", LocationSet());
    }

    bool serialize(tinyxml2::XMLElement* element) const override
    {
        tinyxml2::XMLElement* locationSetElement = element->GetDocument()->NewElement("LocationSet");
        LocationSet locationSet = getData<LocationSet>("LocationSet");
        bool success = locationSet.serialize(locationSetElement);
        element->InsertEndChild(locationSetElement);
        return success;
    }

    bool deserialize(tinyxml2::XMLElement* element) override
    {
        tinyxml2::XMLElement* locationSetElement = element->FirstChildElement("LocationSet");
        if (locationSetElement)
        {
            LocationSet locationSet = getData<LocationSet>("LocationSet");
            bool success = locationSet.deserialize(locationSetElement);
            addData("LocationSet", locationSet);
            return success;
        }
        return false;  // No LocationSet found
    }
};

// File I/O functions
bool loadFromFile(const std::string& filename, Imput& imput)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: Failed to load file " << filename << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Imput");
    if (!root)
    {
        std::cerr << "Error: <Imput> element not found" << std::endl;
        return false;
    }

    return imput.deserialize(root);
}

bool saveToFile(const std::string& filename, const Imput& imput)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Imput");
    bool success = imput.serialize(root);
    doc.InsertEndChild(root);

    if (doc.SaveFile(filename.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: Failed to save file " << filename << std::endl;
        return false;
    }
    return success;
}

// Main function
int main()
{
    Imput imput;
    try
    {
        // Test deserialize with missing data
        if (loadFromFile("complex_example2_input.xml", imput))
        {
            std::cout << "Loaded successfully!" << std::endl;
            LocationSet locationSet = imput.getData<LocationSet>("LocationSet");
            Base base = locationSet.getData<Base>("Base");
            std::vector<Platform> platforms = locationSet.getData<std::vector<Platform>>("Platforms");

            std::cout << "Base Code: " << base.getData<std::string>("Code") << std::endl;
            std::cout << "Base ID: " << base.getData<std::string>("ID") << std::endl;
            for (const auto& platform : platforms)
            {
                std::cout << "Platform Code: " << platform.getData<std::string>("Code")
                    << ", ID: " << platform.getData<std::string>("ID") << std::endl;
            }

            // Test serialize
            if (saveToFile("complex_example2_output.xml", imput))
            {
                std::cout << "Saved successfully to data/output.xml!" << std::endl;
            }
            else
            {
                std::cout << "Save failed" << std::endl;
            }
        }
        else
        {
            std::cout << "Load failed" << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}