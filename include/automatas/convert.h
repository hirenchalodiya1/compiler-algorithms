#ifndef COMPILER_ALGORITHMS_CONVERT_H
#define COMPILER_ALGORITHMS_CONVERT_H

#include <set>
#include <queue>

template<class STATE, class ALPHABET>
DFA<std::set<STATE>, ALPHABET> NFAtoDFA(NFA<STATE, ALPHABET>& nfa){
    DFA<std::set<STATE>, ALPHABET> dfa;
    std::set<STATE> nfa_final = nfa.getFinalStates();
    // both has same sigma
    dfa.setSigma(nfa.getSigma());

    // starting state of DFA
    std::set<STATE> nfa_start = {nfa.getStartingState()};
    std::set<STATE> dfa_start = nfa.getEpsilonClosure(nfa_start);
    dfa.setStartingState(dfa_start);
    dfa.addState(dfa_start);
    for(auto s:dfa_start){
        if(nfa_final.find(s) != nfa_final.end()){
            dfa.addFinalState(dfa_start);
            break;
        }
    }

    // add transition table and new states and final states in dfa
    std::queue<std::set<STATE>> qu;
    std::set<STATE> temp, temp1;
    qu.push(dfa_start);
    while(not qu.empty()){
        temp.clear();

        temp = qu.front();
        qu.pop();

        for(auto alphabet:dfa.getSigma()){ // for each alphabet
            temp1.clear();

            for(auto state:temp){
                for(auto new_state:nfa.getTransition(state, alphabet)){
                    temp1.insert(new_state);
                }
            }

            temp1 = nfa.getEpsilonClosure(temp1);
            dfa.addTransition(temp, alphabet, temp1);

            if(not dfa.isValidState(temp1)){
                dfa.addState(temp1);

                for(auto s:temp1){
                    if(nfa_final.find(s) != nfa_final.end()){
                        dfa.addFinalState(temp1);
                        break;
                    }
                }

                qu.push(temp1);
            }

        }
    }

    return dfa;
}
#endif //COMPILER_ALGORITHMS_CONVERT_H
