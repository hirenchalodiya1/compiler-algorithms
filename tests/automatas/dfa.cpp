#include <iostream>
#include <fstream>
#include <prettyprint/print.h>
#include <automatas/dfa.h>
#include <automatas/create.h>

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/dfa_in");
    DFA<string, string> dfa = createDFA(fin);
    cout << dfa;
    fin.close();
    return 0;
}