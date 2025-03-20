#include "pch.h"
#include "tinyxml2.h"
#include "XMLSerializable.h"
#include "XmlElementWrapper.h"
#include <iostream>
#include <vector>
#include <string>

// This test program demonstrates XML serialization and deserialization of a School structure
// containing Students with Grades, using the XMLSerializable framework. It creates a School
// with two Students (Alice and Bob), saves it to an XML file, loads it back, and verifies
// the data by printing it to the console. The resulting XML matches the specified structure:
// <School><students><Student><name/><grades><math/><science/></grades></Student>...</students></School>

// Grades class
class Grades : public XMLSerializable {
public:
    Grades() {
        addData("math", 0.0);
        addData("science", 0.0);
    }

    Grades(double math, double science) {
        addData("math", math);
        addData("science", science);
    }
};

// Student class
class Student : public XMLSerializable {
public:
    Student() {
        addData("name", std::string(""));
        addData("Grades", Grades());
    }

    Student(const std::string& name, double math, double science) {
        addData("name", name);
        addData("Grades", Grades(math, science));
    }

    bool serialize(tinyxml2::XMLElement* element) const override {
        bool allSuccess = true;
        // Serialize name directly
        tinyxml2::XMLElement* nameElement = element->GetDocument()->NewElement("name");
        nameElement->SetText(getData<std::string>("name").c_str());
        element->InsertEndChild(nameElement);

        // Serialize grades
        tinyxml2::XMLElement* gradesElement = element->GetDocument()->NewElement("grades");
        Grades grades = getData<Grades>("Grades");
        allSuccess &= grades.serialize(gradesElement);
        element->InsertEndChild(gradesElement);

        return allSuccess;
    }

    bool deserialize(tinyxml2::XMLElement* element) override {
        bool allSuccess = true;

        // Deserialize name
        tinyxml2::XMLElement* nameElement = element->FirstChildElement("name");
        if (nameElement && nameElement->GetText()) {
            addData("name", std::string(nameElement->GetText()));
        }
        else {
            allSuccess = false;
        }

        // Deserialize grades
        tinyxml2::XMLElement* gradesElement = element->FirstChildElement("grades");
        if (gradesElement) {
            Grades grades = getData<Grades>("Grades");
            allSuccess &= grades.deserialize(gradesElement);
            addData("Grades", grades);
        }
        else {
            allSuccess = false;
        }
        return allSuccess;
    }
};

// School class
class School : public XMLSerializable {
public:
    School() {
        addData("Students", std::vector<Student>());
    }

    void addStudent(const Student& student) {
        auto students = getData<std::vector<Student>>("Students");
        students.push_back(student);
        addData("Students", students);
    }

    bool serialize(tinyxml2::XMLElement* element) const override {
        bool allSuccess = true;
        tinyxml2::XMLElement* studentsElement = element->GetDocument()->NewElement("students");

        std::vector<Student> students = getData<std::vector<Student>>("Students");
        for (const auto& student : students) {
            tinyxml2::XMLElement* studentElement = element->GetDocument()->NewElement("Student");
            allSuccess &= student.serialize(studentElement);
            studentsElement->InsertEndChild(studentElement);
        }
        element->InsertEndChild(studentsElement);
        return allSuccess;
    }

    bool deserialize(tinyxml2::XMLElement* element) override {
        bool allSuccess = true;
        tinyxml2::XMLElement* studentsElement = element->FirstChildElement("students");

        if (studentsElement) {
            std::vector<Student> students;
            for (tinyxml2::XMLElement* studentElement = studentsElement->FirstChildElement("Student");
                studentElement != nullptr;
                studentElement = studentElement->NextSiblingElement("Student")) {
                Student student;
                allSuccess &= student.deserialize(studentElement);
                students.push_back(student);
            }
            addData("Students", students);
        }
        return allSuccess;
    }
};

bool saveToFile(const std::string& filename, const School& school) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("School");
    bool success = school.serialize(root);
    doc.InsertEndChild(root);

    if (doc.SaveFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error: Failed to save file " << filename << std::endl;
        return false;
    }
    return success;
}

bool loadFromFile(const std::string& filename, School& school) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error: Failed to load file " << filename << std::endl;
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("School");
    if (!root) {
        std::cerr << "Error: <School> element not found" << std::endl;
        return false;
    }

    return school.deserialize(root);
}

int main() {
    try {
        // Create and populate school
        School school;
        school.addStudent(Student("Alice", 95.5, 88.0));
        school.addStudent(Student("Bob", 87.0, 92.5));

        // Test serialize
        if (saveToFile("school_output.xml", school)) {
            std::cout << "Saved successfully to school_output.xml!\n";

            // Test deserialize
            School loadedSchool;
            if (loadFromFile("school_output.xml", loadedSchool)) {
                std::cout << "Loaded successfully!\n";

                // Print results
                auto students = loadedSchool.getData<std::vector<Student>>("Students");
                for (const auto& student : students) {
                    std::string name = student.getData<std::string>("name");
                    Grades grades = student.getData<Grades>("Grades");
                    double math = grades.getData<double>("math");
                    double science = grades.getData<double>("science");

                    std::cout << "Student: " << name << "\n";
                    std::cout << "  Math: " << math << "\n";
                    std::cout << "  Science: " << science << "\n";
                }
            }
            else {
                std::cout << "Load failed\n";
            }
        }
        else {
            std::cout << "Save failed\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}