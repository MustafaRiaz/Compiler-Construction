#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void printNumbers1() {
    for (int i = 1; i <= 5; ++i) {
        cout << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void printNumbers2() {
    for (int i = 6; i <= 10; ++i) {
        cout << i << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    thread t1(printNumbers1);
    thread t2(printNumbers2);
    t1.join();
    t2.join();
    return 0;
}
