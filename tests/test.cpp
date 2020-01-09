#include <iostream>
#include <vector>
#include <prettyprint/print.cpp>
#include <automatas/dfa.h>

using namespace std;
using namespace prettyprint;
int main() {
    DFA<int, int> dfa;
    cout << dfa;
    return 0;
}