// Ghulam Mustafa (2021-CS-39)

#include <iostream>
#include <deque>

using namespace std;

int main(){
    deque<string> webpages;

    string q="";
    int count=0;
    while(q!="E"){
        cout<<"Press V to visit webpage, B to go back, and E to exit"<<endl;
        cin>>q;
        if(q=="V"){
            count++;
            webpages.push_back("Page");
            cout<<"Visited: "<<webpages.back()<<" "<<count<<endl;
        }
        if(q=="B"){
            if(webpages.empty()){
                return false;
            }
            count--;
            webpages.pop_front();
            cout<<"Back to: "<<webpages.front()<<" "<<count<<endl;
        }
        
    }


    
    return 0;
}