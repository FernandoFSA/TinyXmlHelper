#include "XmlElementWrapper.h"
#include "XMLSerializable.h"
#include <tinyxml2.h>
#include <iostream>

class Person : public XMLSerializable {
public:
    Person() {
        addData("name", std::string(""));
        addData("age", 0);
    }
};

int main() {
    // Create an XML document
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("root");
    doc.InsertFirstChild(root);

    // Serialize
    Person person;
    person.addData("name", std::string("Alice"));
    person.addData("age", 25);
    XmlElementWrapper wrapper(root, "person", true);
    wrapper << person;

    // Save to file
    doc.SaveFile("output.xml");

    // Deserialize
    tinyxml2::XMLDocument doc2;
    doc2.LoadFile("output.xml");
    Person person2;
    XmlElementWrapper wrapper2(doc2.FirstChildElement("root"), "person");
    wrapper2 >> person2;

    std::cout << "Name: " << person2.getData<std::string>("name") << "\n";
    std::cout << "Age: " << person2.getData<int>("age") << "\n";
    return 0;
}