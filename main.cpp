#include <iostream>
#include "predictiveparsing.h"
#include <fstream>
#include <vector>
#include <string>
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
    PredictiveParsing parser(inFile, "?");
    vector<string> st = {"d", "d"};
    cout << parser.check_string(st);
    return 0;
}