#include <iostream>
#include "cfg.h"
#include <fstream>
using namespace std;
int main(int n, char** c) {
    ifstream inFile;
    inFile.open(c[1], ios::in);
    CFG gram(inFile);
    cout << gram;
    for(auto i:gram.nonterminals()){
        cout<< i<<endl;
    }
    return 0;
}