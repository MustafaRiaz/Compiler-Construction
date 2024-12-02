#include<iostream>
using namespace std;

struct Node{
    string url;
    Node *prev,*next;
    Node(string u):url(u),prev(nullptr),next(nullptr){}
};

class BrowserHistory{
    Node *head,*tail,*current;
public:
    BrowserHistory():head(nullptr),tail(nullptr),current(nullptr){}

    void addPage(string url){
        Node *newPage=new Node(url);
        if(!head)head=tail=current=newPage;
        else{
            current->next=newPage;
            newPage->prev=current;
            current=newPage;
            tail=newPage;
        }
    }

    void moveForward(){
        if(current&&current->next)current=current->next;
        else cout<<"No forward page!\n";
    }

    void moveBackward(){
        if(current&&current->prev)current=current->prev;
        else cout<<"No backward page!\n";
    }

    void deletePage(){
        if(!current)return;
        if(current==head)head=current->next;
        if(current==tail)tail=current->prev;
        if(current->prev)current->prev->next=current->next;
        if(current->next)current->next->prev=current->prev;
        Node* temp=current;
        current=(current->prev)?current->prev:current->next;
        delete temp;
    }

    void printCurrent(){cout<<"Current page: "<<(current?current->url:"None")<<endl;}
};

int main(){
    BrowserHistory bh;
    bh.addPage("page1.com");
    bh.addPage("page2.com");
    bh.addPage("page3.com");
    bh.printCurrent();
    bh.moveBackward();bh.printCurrent();
    bh.moveForward();bh.printCurrent();
    bh.deletePage();bh.printCurrent();
    return 0;
}
