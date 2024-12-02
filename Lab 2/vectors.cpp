#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<int> numbers;
    numbers.push_back(6);
    numbers.push_back(4);
    numbers.push_back(2);
    cout<< "This is the first number"<< numbers[1];
    return 0;
}