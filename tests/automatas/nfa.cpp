#include <iostream>
#include <fstream>
#include <prettyprint/print.h>
#include <automatas/nfa.h>
#include <automatas/create.h>

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/nfa_in");
    NFA<string, string> nfa = createNFA(fin);
    cout << nfa;
    fin.close();
    return 0;
}