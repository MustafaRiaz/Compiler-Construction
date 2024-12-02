#include <iostream>
#include <string>
using namespace std;

int main() {
    int count = 0;
    string test_code = "main(){int a=10;}";
    int length = test_code.length();
    string token = "";
    for (int i = 0; i < length; i++) {
        if(test_code[i]=='('|| test_code[i]==')'||test_code[i]=='{'||test_code[i]=='}'||test_code[i]==';'|| test_code[i]==','|| test_code[i]==' '|| test_code[i]=='='){
            if (token!="") {
                count++;
                cout<<token<<"\n";
                token="";
            }
            if (test_code[i]!=' '){
                count++;
                cout<<test_code[i]<< "\n";
            }
        } else {
            token+=test_code[i];
        }
    }
    if (token != "") {
        count++;
        cout<<token;
    }
    count<<count;
    return 0;
}
