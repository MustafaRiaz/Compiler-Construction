#include <iostream>
using namespace std;
struct PersonInfo {string name;
int age;
};
class ClassroomUser{
protected:
    PersonInfo info;
public:
    ClassroomUser(const string& name,int age):info{name,age} {}
    virtual void displayInfo() const{
    cout<<"Name: "<<info.name <<" Age: "<<info.age;
    }
    virtual ~ClassroomUser() {}
};
class Student:public ClassroomUser {
    string studentID;
public:
    Student(const string& name,int age,const string& id):ClassroomUser(name,age), studentID(id) {}
    void displayInfo() const override{
    ClassroomUser::displayInfo();
        cout<<", Student ID: "<<studentID<<endl;
    }
};

class Teacher:public ClassroomUser {
    string subject;
public:
    Teacher(const string& name,int age,const string& subject) 
    :ClassroomUser(name, age), subject(subject) {}
    void displayInfo() const override {
        ClassroomUser::displayInfo();
        cout<<" Subject: "<<subject<< endl;
    }
};

int main() {
    ClassroomUser* users[] = {
        new Student("Mustafa", 20, "S123"),
        new Teacher("Saleem", 40, "Math")
    };

    for(auto* user:users) {
        user->displayInfo();
        delete user;
    }
    return 0;
}