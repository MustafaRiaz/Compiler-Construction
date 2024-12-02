#include<iostream>
#include<deque>
using namespace std;

class TaskScheduler{
    deque<string> tasks;
public:
    void addTask(string task,bool highPriority=false){
        if(highPriority)tasks.push_front(task);
        else tasks.push_back(task);
}
    void removeFront(){
        if(!tasks.empty())tasks.pop_front();
        else cout<<"No task to remove from front!\n";
    }
    void removeBack(){
        if(!tasks.empty())tasks.pop_back();
        else cout<<"No task to remove from back!\n";
    }
    void showFront(){
        if(!tasks.empty())cout<<"Front task: "<<tasks.front()<<endl;
        else cout<<"No tasks!\n";
    }
    void showBack(){
        if(!tasks.empty())cout<<"Back task: "<<tasks.back()<<endl;
        else cout<<"No tasks!\n";
    }
};

int main(){
    TaskScheduler ts;
    ts.addTask("Task1");
    ts.addTask("Task2");
    ts.addTask("HighPriorityTask",true);
    ts.showFront(); // HighPriorityTask
    ts.showBack();  // Task2
    ts.removeFront();
    ts.showFront(); // Task1
    ts.removeBack();
    ts.showBack();  // Task1
    return 0;
}
