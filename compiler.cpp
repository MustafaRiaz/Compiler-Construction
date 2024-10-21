#include <iostream>
#include <fstream>
using namespace std;
int main( int argc, char *argv[]){
    
    ifstream file(argv[1]);
    if (!file){
        cout<<"No file was passed to the program";
    }
    else{
        string line;
        while(getline(file, line)){
            cout<<line<<"\n";
        }
    }
    return 0;
}