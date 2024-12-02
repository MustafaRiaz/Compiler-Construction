// Ghulam Mustafa (2021-CS-39)

#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int sum1(int *list,int range){
    int sum=0;
    for(int i=0;i<range;i++){
        sum=list[i]+sum;
    }
    this_thread::sleep_for(chrono::milliseconds(10000));
    return sum;
}
int sum2(int *list,int range,int length){
    int sum=0;
    for(int i=range-1;i<length;i++){
        sum=list[i]+sum;
    }
    this_thread::sleep_for(chrono::milliseconds(10000));
    return sum;
}


int main(){
    int list1[6]={1,2,3,4,5,6};
    int l=6;
    int t =sum1(list1,3);
    int t2 =sum2(list1,4,l);

    thread t(sum1(list1,3));
    thread t2(sum1(list1,3));

    cout<<"Sum of first thread: "<<t<<endl;
    cout<<"Sum of second thread: "<<t2;
    return 0;
}