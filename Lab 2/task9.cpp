#include <iostream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <string>
using namespace std;



void countWords(const string& text,unordered_map<string,int>& MapFrequency) {
    istringstream stream(text);
    string word;
    while (stream >> word) {
        MapFrequency[word]++;
    }
}
void countWords(const string& text,map<string,int>& MapFrequency) {
    istringstream stream(text);
    string word;
    while (stream>>word) {
        MapFrequency[word]++;
    }
}
void printFrequency(const unordered_map<string, int>& MapFrequency) {
    for (const auto& entry : MapFrequency) {
        cout<<entry.first << ": " <<entry.second <<"\n";
    }
}
int main() {
    string text ="Hello my name is Mustafa Riaz, I am from Pindi Bhattian";
    unordered_map<string,int> unorderedMapFrequency;
    countWords(text, unorderedMapFrequency);
    cout<<"Frequency using unordered_map:\n";
    printFrequency(unorderedMapFrequency);
    map<string,int> orderedMapFrequency;
    countWords(text,orderedMapFrequency);
    cout<<"\nFrequency using ordered_map:\n";
    for (const auto& entry:orderedMapFrequency){
        cout<<entry.first<<": "<<entry.second<<endl;
    }
    return 0;
}
