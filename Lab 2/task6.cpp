#include<iostream>
#include<queue>
#include<vector>
using namespace std;


struct Patient{
    string name;
    int severity;

    Patient(string n,int s):name(n),severity(s){}
    bool operator<(const Patient& other)const{
        return severity<other.severity;
    }
};
class EmergencyRoom{
    priority_queue<Patient> pq;
public:
    void addPatient(string name,int severity){
        pq.push(Patient(name,severity));
    }
    void treatPatient(){
        if(pq.empty()){
            cout<<"No patients to treat!\n";
            return;
        }
        Patient p=pq.top();
        pq.pop();
        cout<<"Treating patient: "<<p.name<<" with severity "<<p.severity<<endl;
    }
};

int main(){
    EmergencyRoom er;
    er.addPatient("Patient1",2); 
    er.addPatient("Patient2",5); 
    er.addPatient("Patient3",3); 

    er.treatPatient(); 
    er.treatPatient();
    er.treatPatient();
    er.treatPatient();
    return 0;
}
