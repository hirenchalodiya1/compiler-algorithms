#include <iostream>
#include "syntex_analysis/topdownparser/predictiveparsing.h"
#include <fstream>
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
    ifstream inFile1;
    if(n < 3) {
        inFile1.open("input", ios::in);
    }
    else{
        inFile1.open(c[2], ios::in);
    }
    if(!inFile1.is_open()){
        cout<<"File can not be opened\n";
        exit(1);
    }
    PredictiveParsing parser(inFile, "?");
    parser.check_string(inFile1);
    return 0;
}