//
// Created by hiren on 28/09/19.
//

#ifndef COMPILER_ALGORITHMS_TEST_H
#define COMPILER_ALGORITHMS_TEST_H
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <limits>
#include <stack>
#include <utility>
#include "../../libs/prettyprint/include/print.h"

template <class STATE>
class DFA{
private:
    /* DFA Properties (Σ, Q, δ, q0, F) */
    std::set<std::string> sigma_;
    std::set<STATE*> states_;
    STATE* starting_state_;
    std::set<STATE*> final_states_;
    std::map<STATE*, std::map<std::string, STATE*>> delta_;
    /* helper data structure */
    bool success_;
    /* helper functions */
    void _setDFA(std::istream& is);
    void _makeTransitionTable(std::istream& is);
    bool _isValidSymbol(const std::string &str) const;
    STATE* _getPointer(STATE* t) const;
public:
    explicit DFA(){
        success_ = false;
    };
    explicit DFA(std::istream&is){
        success_ = false;
        _setDFA(is);
    }
    explicit DFA(std::set<std::string> sigma, STATE *startingState):sigma_(std::move(sigma)), success_(true) {
        auto add = *startingState;
        starting_state_ = &add;
        for(auto*i:*starting_state_){
            std::cout << i << " ";
        }
        states_.insert(&add);
    }
    explicit operator bool(){
        return success_;
    }
    /* setter functions */
    bool addTransition(STATE* state1, std::string& str, STATE* state2);
    void addState(STATE* state);
    void addFinalState(STATE* state);
    /* getter functions */
    const std::set<std::string> &getSigma() const;
    const std::set<STATE *> &getStates() const;
    STATE *getStartingState() const;
    const std::set<STATE *> &getFinalStates() const;
    STATE* transition(STATE * state, const std::string& symbol);
    /* operator function */
    template <class T>
    friend std::istream&operator>>(std::istream& is, DFA<T>& dfa);
    template <class T>
    friend std::ostream&operator<<(std::ostream& os, DFA<T>& dfa);
};
/* helper functions */

template <class T>
void DFA<T>::_setDFA(std::istream &is){
    std::string line, temp;
    char temp_char;
    unsigned int i, n;
    // get all symbols
    if(&is == &std::cin){
        is.clear();
        std::cout << "Enter all symbols separated by space and then hit enter:\n";
    }
    std::getline(is, line);
    line.erase(line.find_last_not_of("\r\n") + 1);
    i = 0;
    n = line.size();
    while(i < n){
        temp.clear();
        while (i < n and std::isspace(line[i])){
            i++;
        }
        while (i < n and std::isalnum(line[i])){
            temp.push_back(line[i]);
            i++;
        }
        if(!temp.empty()){
            sigma_.emplace(temp);
        }
    }
    if(!(success_ = !sigma_.empty())){
        return;
    }
    T *t;
    // get all states
    if(&is == &std::cin){
        std::cout << "Enter all states separated by space and then hit enter:\n";
    }
    temp_char = is.get();
    if(!(success_ = (temp_char != EOF))){
        return;
    }
    is.putback(temp_char);
    while (temp_char != '\n'){
        t = new T();
        is >> *t;
        states_.emplace(t);
        temp_char = is.get();
        if(!(success_ = (temp_char != EOF))){
            return;
        }
    }
    if(!(success_ = !states_.empty())){
        return;
    }
    // get staring state
    if(&is == &std::cin){
        std::cout << "Enter starting state and then hit enter: ";
    }
    t = new T();
    is >> *t;
    starting_state_  = _getPointer(t);
    if(!starting_state_){
        success_ = false;
        return;
    }
    if(!(success_ = (is.get() == '\n'))){
        return;
    }
    // final states
    if(&is == &std::cin){
        std::cout << "Enter all final states separated by space and then hit enter:\n";
    }
    temp_char = is.get();
    if(!(success_ = (temp_char != EOF))){
        return;
    }
    is.putback(temp_char);
    while (temp_char != '\n'){
        t = new T();
        is >> *t;
        final_states_.emplace(_getPointer(t));
        temp_char = is.get();
        if(!(success_ = (temp_char != EOF))){
            return;
        }
    }
    if(!(success_ = !final_states_.empty())){
        return;
    }
    _makeTransitionTable(is);
}

template <class T>
void DFA<T>::_makeTransitionTable(std::istream &is) {
    // transition table
    if(&is == &std::cin){
        std::cout << "Enter all transitions in this format <STATE><SPACE>, <SYMBOL><SPACE> - <STATES SEPARATED BY SPACE AND HIT ENTER>:\n";
        std::cout << "Press CTRL + D (EOF) to end stream\n";
    }

    const char arrow = '-';
    const char delimiter= ',';

    std::string line, temp, symbol;
    T* t = new T();
    char temp_char;
    while (std::getline(is, line)) {
        line.erase(line.find_last_not_of("\r\n") + 1);
        std::stringstream s(line);
        s >> *t;
        T* leftPoint = _getPointer(t);
        if(!leftPoint){continue;}
        s >> temp_char;
        if(std::isspace(temp_char)){s >> temp_char;}
        if(temp_char == EOF){continue;}
        if(temp_char != delimiter){continue;}
        s >> symbol;
        s >> temp_char;
        if(!_isValidSymbol(symbol)){continue;}
        if(std::isspace(temp_char)){s >> temp_char;}
        if(temp_char == EOF){continue;}
        if(temp_char != arrow){continue;}
        s >> *t;
        T* rightPoint = _getPointer(t);
        if(!rightPoint)continue;
        addTransition(leftPoint, symbol, rightPoint);
    }
}

template<class T>
T *DFA<T>::_getPointer(T *t) const {
    for(const auto i:states_){
        if((*i)==(*t)){
            return i;
        }
    }
    return nullptr;
}

template<class T>
bool DFA<T>::_isValidSymbol(const std::string &str) const {
    return sigma_.find(str) != sigma_.end();
}
/* setter functions */

template<class T>
bool DFA<T>::addTransition(T *state1, std::string& str, T *state2) {
    if(states_.find(state1) == states_.end() or states_.find(state2) == states_.end()){
        return false;
    }
    delta_[state1][str] = state2;
    return true;
}

template<class STATE>
void DFA<STATE>::addState(STATE *state) {
    states_.insert(state);
}

template<class STATE>
void DFA<STATE>::addFinalState(STATE *state) {
    states_.insert(state);
    final_states_.insert(state);
}

/* operator functions */
template<class T>
std::istream &operator>>(std::istream &is, DFA<T> &dfa) {
    dfa._setDFA(is);
    if(&is == &std::cin){
        std::cout << "\n";
    }
    return is;
}

template<class T>
std::ostream &operator<<(std::ostream &os, DFA<T> &dfa) {
    using namespace prettyprint;
    prettyprint::make_default();
    auto start = *dfa.getStartingState();
    os << "------------------- DFA -------------------------\n"
       << "Symbols:\n\t" << dfa.sigma_ << "\b\n"
       << "Starting State: " << start << "\n"
       << "States:\n\t" << dfa.states_ << "\b\n"
       << "Final states:\n\t" << dfa.final_states_ << "\b\n"
       << "Delta Transitions:\n" << dfa.delta_
       << "-------------------------------------------------\n";
    return os;
}

/* getter functions */
template<class STATE>
const std::set<std::string> &DFA<STATE>::getSigma() const {
    return sigma_;
}

template<class STATE>
const std::set<STATE *> &DFA<STATE>::getStates() const {
    return states_;
}

template<class STATE>
STATE *DFA<STATE>::getStartingState() const {
    return starting_state_;
}

template<class STATE>
const std::set<STATE *> &DFA<STATE>::getFinalStates() const {
    return final_states_;
}

template<class T>
T* DFA<T>::transition(T *state, const std::string &symbol) {
    return delta_[state][symbol];
}

#endif //COMPILER_ALGORITHMS_TEST_H
