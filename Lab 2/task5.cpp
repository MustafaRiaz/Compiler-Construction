#include<iostream>
#include<queue>
using namespace std;



class TicketingSystem{
    queue<string> regularQueue;
    queue<string> vipQueue;
public:
    void joinQueue(string customer,bool isVIP=false){
        if(isVIP)vipQueue.push(customer);
        else regularQueue.push(customer);
    }
    void bookTicket(){
        if(!vipQueue.empty()){
            cout<<"Serving VIP: "<<vipQueue.front()<<endl;
            vipQueue.pop();
        }else if(!regularQueue.empty()){
            cout<<"Serving Regular: "<<regularQueue.front()<<endl;
            regularQueue.pop();
        }else{
            cout<<"No customers in queue!\n";
        }
    }
};
int main(){
    TicketingSystem ts;
    ts.joinQueue("Customer1");
    ts.joinQueue("VIP1",true);
    ts.joinQueue("Customer2");
    ts.bookTicket(); 
    ts.bookTicket(); 
    ts.bookTicket(); 
    ts.bookTicket(); 
    return 0;
}
