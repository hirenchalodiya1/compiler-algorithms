//
// Created by hiren on 09/01/20.
//

#include "dfa.h"


/* IOS overloaded for DFA class */

template<class STATE, class ALPHABET>
std::ostream&operator<<(std::ostream& os, DFA<STATE, ALPHABET>& dfa){
    os << "------------------------------DFA------------------------------"
       << "Symbols:\n\t" << dfa.sigma_ << "\b\n"
       << "Starting State: " << dfa.starting_state_ << "\n"
       << "States:\n\t" << dfa.states_ << "\b\n"
       << "Final states:\n\t" << dfa.final_states_ << "\b\n"
       << "Delta Transitions:\n" << dfa.delta_
       << "---------------------------------------------------------------";
    return os;
}
template<class STATE, class ALPHABET>
std::istream&operator>>(std::istream& is, DFA<STATE, ALPHABET>& dfa){
    return is;
}

#include<iostream>

using namespace std;
int main(){
    DFA<int> dfa;
    if(dfa){
        printf("Hi there, dfa is valid");
        cout << dfa;
    }
    else{
        printf("DFA not valid");
        cout << dfa;
    }
}