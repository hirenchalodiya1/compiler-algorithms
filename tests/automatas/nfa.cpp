#include <fstream>
#include <sstream>
#include <prettyprint/print.h>
#include <automatas/nfa.h>
#include <automatas/create.h>
#include "../main.cpp"

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/nfa_in");
    NFA<string, string> nfa = createNFA(fin);
    fin.close();

    std::stringstream output;
    output << nfa;

    ASSERT_STR_STREAM_AND_FILE ( output , "../tests/expected_output/nfa_out" );

    return 0;
}