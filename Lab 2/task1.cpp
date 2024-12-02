
#include <iostream>
#include <vector>
using namespace std;
int insertGrades(vector<int>& grades, int grade){
    grades.push_back(grade);
    cout<<"Grade is added to the vector array\n";
    return 0;
}
void removeGrades(vector<int>& grades, int index){
    cout<<"The grade at the index: "<<index<<" will be removed which is: "<<grades.at(index);
    grades.erase(grades.begin()+index);
}
int findElement(vector<int>& grades, int index){
    int x=grades.at(index);
    cout<<"The element found is: "<<x;
    return x;
}
void showElements(vector<int>& grades){
    for(int grade:grades){
        cout<<"\nThe elements in array are: "<<grade<<"\n";
    }
}

int main() {
    vector<int> grades;
    insertGrades(grades,8);
    insertGrades(grades,6);
    insertGrades(grades,7);
    showElements(grades);
    removeGrades(grades,2);
    showElements(grades);
    findElement(grades,1);
    
}