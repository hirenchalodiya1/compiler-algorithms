//
// Created by hiren on 28/09/19.
//

#ifndef COMPILER_ALGORITHMS_CONVERT_H
#define COMPILER_ALGORITHMS_CONVERT_H

#include "dfa_generic.h"
#include "nfa_generic.h"
#include <set>

namespace convert{
    template <class T>
    std::set<T> _pointerToObject(std::set<T*> &s){
        std::set<T> ret;
        for(auto*i:s){
            ret.insert(*i);
        }
        return ret;
    }
    template <class T>
    std::set<T*> merge(T* state1, T* state2){
        return std::set<T*>{state1, state2};
    }
    template <class STATE>
    DFA<std::set<STATE*>>* NFAToDFA(NFA<STATE>* nfa, std::set<STATE*>(*mergeFunc)(STATE*, STATE*)){
        // states
        // starting state
        // final states
        std::set<STATE*> sett;
        sett.insert(nfa->getStartingState());
        auto start = nfa->getEpsilonClosure(sett);
        for(auto*i:start){
            std::cout << *i << " ";
        }
        return new DFA<std::set<STATE*>>(nfa->getSigma(), &start);
    }
    template <class STATE>
    DFA<std::set<STATE*>>* NFAToDFA(NFA<STATE>& nfa, std::set<STATE*>(*mergeFunc)(STATE*, STATE*)){
        return NFAToDFA(&nfa, mergeFunc);
    }
    template <class STATE>
    DFA<std::set<STATE*>>* NFAToDFA(NFA<STATE>* nfa){
        return NFAToDFA(nfa, merge);
    }
    template <class STATE>
    DFA<std::set<STATE*>>* NFAToDFA(NFA<STATE>& nfa){
        return NFAToDFA(&nfa);
    }
}
#endif //COMPILER_ALGORITHMS_CONVERT_H
