#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
using namespace std;

bool isKeyword(string token){
    string keywords[]={"int","float","bool","if","else","while","do","for","return","void"};
    for(int i=0;i<10;i++){
        if(token==keywords[i]){
            return true;
        }
    }
    return false;
}

bool isOperator(string token){
    string operators[]={"+","-","*","=",">","<","<=",">=","==","!="};
    for(int i=0;i<10;i++){
        if(token==operators[i]){
            return true;
        }
    }
    return false;
}

bool isPunctuator(string token){
    string punctuators[]={",","{","}","(",";",")"};
    for(int i=0;i<6;i++){
        if(token==punctuators[i]){
            return true;
        }
    }
    return false;
}

bool isIdentifier(string token){
    if((token[0]>='a' && token[0]<='z')||(token[0]>='A' && token[0]<='Z')||token[0]=='_'){
        return true;
    }
    return false;
}

bool isNumber(string token){
    for(char c:token){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;
}

void tokenize(const string& str, vector<string>& tokens){
    string token;
    for(char c:str){
        if(isspace(c)||ispunct(c)){
            if(!token.empty()){
                tokens.push_back(token);
                token.clear();
            }
            if(ispunct(c)){
                tokens.push_back(string(1,c));
            }
        } else{
            token+=c;
        }
    }
    if(!token.empty()){
        tokens.push_back(token);
    }
}

int main(){
    string test_code2="main(){\nint a=10;\n}";
    string test_code="for(int i=0;i<6;i++){\nif(token==punctuators[i]){\nreturn true;\n}\n}";
    cout<<"This is the test code : "<<test_code<<"\n";
    vector<string> tokens;

    tokenize(test_code,tokens);
    cout<<"Results: \n";
    int count=0;
    for(const string& token:tokens){

        if(isKeyword(token)){

            cout<<token<<" keyword"<<endl;
            count++;
        }
        else if(isIdentifier(token)){
            cout<<token<<" identifier"<<endl;
            count++;
        }
        else if(isOperator(token)){
            cout<<token<<" operator"<<endl;
            count++;
        }
        else if(isPunctuator(token)){
            cout<<token<<" punctuator"<<endl;
            count++;
        }
        else if(isNumber(token)){
            cout<<token<<" number"<<endl;
            count++;
        }
        else{
            cout<<token<<" is unknown"<<endl;
            count++;
        }
    
    }
    cout<<"Total tokens: "<<count<<endl;
    return 0;
}
