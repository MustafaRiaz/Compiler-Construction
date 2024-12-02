#include<iostream>
#include<set>
using namespace std;



int main(){
    set<string> emails;
    emails.insert("maxie@gmail.com");
    emails.insert("Mustafa@gmail.com");
    emails.insert("Saleem@gmail.com"); // Duplicate, won't be added
    emails.insert("Ahmed@gmail.com");

    cout<<"Unique Emails:\n";
    for(const string& email:emails)
        cout<<email<<endl;

    string searchEmail="Ahmed@gmail.com";
    if(emails.find(searchEmail)!=emails.end())
        cout<<searchEmail<<" found\n";
    else
        cout<<searchEmail<<" not found\n";

    emails.erase("Mustafa@gmail.com");
    cout<<"After deletion of Mustafa@gmail.com:\n";
    for(const string& email:emails)
        cout<<email<<endl;
    return 0;
}
