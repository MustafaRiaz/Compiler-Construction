#include <iostream>
#include <map>
using namespace std;



struct Student {
    string name;
    double grade;
    Student():name(""),grade(0.0) {}
    Student(string n, double g) : name(n), grade(g){}
    Student(const Student& other):name(other.name),grade(other.grade){}
};
class StudentRecordSystem {
    map<int,Student> records;
public:
    void addStudent(int id, string name, double grade) {
        records[id] = Student(name, grade);
    }
    void deleteStudent(int id) {
        records.erase(id);
    }
    void getStudent(int id) {
        auto it= records.find(id);
        if (it!=records.end()) {
            cout<<"ID: " <<id<< " Name: "<<it->second.name <<" Grade: " << it->second.grade<<"\n";
        } else {
            cout<<"Student ID "<<id<< " not found\n";
        }
    }
};

int main() {
    StudentRecordSystem srs;
    srs.addStudent(101, "Mustafa", 85.5);
    srs.addStudent(102, "Saleem", 92.0);
    srs.addStudent(103, "Maxie", 78.3);
    srs.getStudent(102);
    srs.deleteStudent(103);
    srs.getStudent(103);
    return 0;
}
