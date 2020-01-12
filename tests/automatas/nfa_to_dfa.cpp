#include <iostream>
#include <fstream>
#include <prettyprint/print.h>
#include <automatas/nfa.h>
#include <automatas/dfa.h>
#include <automatas/create.h>
#include <automatas/convert.h>

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/nfatodfa_in");
    NFA<string, string> nfa = createNFA(fin);
    DFA<set<string>, string> dfa = NFAtoDFA(nfa);
    cout << nfa;
    cout << dfa;
    fin.close();
    return 0;
}