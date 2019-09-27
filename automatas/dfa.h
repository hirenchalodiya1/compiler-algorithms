//
// Created by hiren on 28/09/19.
//

#ifndef COMPILER_ALGORITHMS_DFA_H
#define COMPILER_ALGORITHMS_DFA_H
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <limits>
#include <stack>
#include <utility>
#include "../print.h"

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
    explicit DFA(std::set<std::string> sigma, const std::set<STATE *> &states, STATE *startingState,
                 const std::set<STATE *> &finalStates):sigma_(std::move(sigma)),states_(states), starting_state_(std::move(startingState)), final_states_(finalStates), success_(true) {};
    explicit operator bool(){
        return success_;
    }
    /* public functions */
    bool addTransition(STATE* state1, std::string& str, STATE* state2);
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
/* public functions */
template<class T>
bool DFA<T>::addTransition(T *state1, std::string& str, T *state2) {
    if(states_.find(state1) == states_.end() or states_.find(state2) == states_.end()){
        return false;
    }
    delta_[state1][str] = state2;
    return true;
}
template<class T>
T* DFA<T>::transition(T *state, const std::string &symbol) {
    return delta_[state][symbol];
}
/* operator functions */
template<class T>
std::istream &operator>>(std::istream &is, DFA<T> &dfa) {
    if(&is == &std::cin){
        std::cout << "Enter epsilon value: ";
        is >> dfa.epsilon_;
        is.ignore(reinterpret_cast<std::streamsize>(std::numeric_limits<std::streamsize>::max), '\n');
    }
    dfa._setDFA(is);
    return is;
}
template<class T>
std::ostream &operator<<(std::ostream &os, DFA<T> &dfa) {
    using namespace prettyprint;
    prettyprint::make_default();
    os << "------------------- DFA -------------------------\n"
       << "Symbols:\n\t" << dfa.sigma_ << "\b\n"
       << "Starting State: " << *dfa.starting_state_ << "\n"
       << "States:\n\t" << dfa.states_ << "\b\n"
       << "Final states:\n\t" << dfa.final_states_ << "\b\n"
       << "Delta Transitions:\n" << dfa.delta_
       << "-------------------------------------------------\n";
    return os;
}
#endif //COMPILER_ALGORITHMS_DFA_H
