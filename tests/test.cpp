#include <iostream>
#include "../include/print.h"
#include "syntex_analysis/topdownparser/predictiveparser.h"
#include <fstream>
using namespace std;
using namespace prettyprint;
class Item{
public:
    string st;
public:
    friend istream& operator >>(istream& is, Item& it){
        is >> it.st;
        return is;
    }
    friend ostream& operator <<(ostream& os,const Item& it){
        os << it.st;
        return os;
    }
};
bool operator == (const Item& it1, const Item& it2){
    return it1.st == it2.st;
}
int main(int n, char** c) {
    ifstream inFile;
    inFile.open("input2", ios::in);
    if(!inFile.is_open()) {
        cout << "File can not be opened\n";
        exit(1);
    }
    PredictiveParser parser(inFile, "?");
    cout << *parser.getGram();
    return 0;
}