#include <iostream>
#include "firstfollow.h"
#include <fstream>
using namespace std;
int main(int n, char** c) {
    ifstream inFile;
    if(n < 2) {
        inFile.open("input2", ios::in);
    }
    else{
        inFile.open(c[1], ios::in);
    }
    if(!inFile.is_open()){
        cout<<"File can not be opened\n";
        exit(1);
    }
    PredictiveParsingTable table(inFile, "?");
    ofstream outFile;
    outFile.open("output", ios::out);
    outFile << table;
    return 0;
}