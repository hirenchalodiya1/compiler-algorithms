#include <fstream>
#include <sstream>
#include <prettyprint/print.h>
#include <automatas/nfa.h>
#include <automatas/dfa.h>
#include <automatas/create.h>
#include <automatas/convert.h>
#include "../main.cpp"

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/nfatodfa_in");
    NFA<string, string> nfa = createNFA(fin);
    DFA<set<string>, string> dfa = NFAtoDFA(nfa);
    fin.close();

    std::stringstream output;
    output << nfa;
    output << dfa;

    ASSERT_STR_STREAM_AND_FILE ( output , "../tests/expected_output/nfatodfa_out" );

    return 0;
}