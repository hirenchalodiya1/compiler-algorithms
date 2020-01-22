#include <fstream>
#include <sstream>
#include <prettyprint/print.h>
#include <automatas/dfa.h>
#include <automatas/create.h>
#include "../main.cpp"

using namespace std;
using namespace prettyprint;
int main() {
    ifstream fin;
    fin.open("../tests/inputs/dfa_in");
    DFA<string, string> dfa = createDFA(fin);
    fin.close();

    std::stringstream output;
    output << dfa;

    ASSERT_STR_STREAM_AND_FILE ( output , "../tests/expected_output/dfa_out" );

    return 0;
}