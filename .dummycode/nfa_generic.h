//
// Created by hiren on 26/09/19.
//

#ifndef COMPILER_ALGORITHMS_NFA_H
#define COMPILER_ALGORITHMS_NFA_H

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
class NFA{
private:
    /* NFA Properties (Σ, Q, δ, q0, F) */
    std::set<std::string> sigma_;
    std::set<STATE*> states_;
    STATE* starting_state_;
    std::set<STATE*> final_states_;
    std::map<STATE*, std::map<std::string, std::set<STATE*>>> delta_;
    /* helper data structure */
    std::string epsilon_;
    bool success_;
    /* helper functions */
    void _setNFA(std::istream& is);
    void _makeTransitionTable(std::istream& is);
    STATE* _getPointer(STATE* t) const;
    bool _isValidSymbol(const std::string &str) const;
public:
    explicit NFA(std::string e="ep"):epsilon_(std::move(e)){
        success_ = false;
    };
    explicit NFA(std::istream&is, std::string e="ep"):epsilon_(std::move(e)){
        success_ = false;
        _setNFA(is);
    }
    explicit NFA(std::set<std::string> sigma, STATE *startingState, std::string epsilon="ep"):sigma_(std::move(sigma)), starting_state_(std::move(startingState)), epsilon_(std::move(epsilon)), success_(true) {
        states_.insert(startingState);
    };
    explicit operator bool(){
        return success_;
    }
    /* public functions */
    bool addTransition(STATE* state1, std::string& str, STATE* state2);
    void addState(STATE* state);
    void addFinalState(STATE* state);
    /* getter functions */
    std::set<STATE *> getEpsilonClosure(std::set<STATE *>& states);
    std::set<STATE *> transition(std::set<STATE *> states, const std::string& symbol);
    std::set<STATE *> transition(STATE * state, const std::string& symbol);
    const std::set<std::string> &getSigma() const;
    const std::set<STATE *> &getStates() const;
    STATE *getStartingState() const;
    const std::set<STATE *> &getFinalStates() const;
    const std::string &getEpsilon() const;
    /* operator function */
    template <class T>
    friend std::istream&operator>>(std::istream& is, NFA<T>& nfa);
    template <class T>
    friend std::ostream&operator<<(std::ostream& os, NFA<T>& nfa);
};

/* helper functions */

template <class T>
void NFA<T>::_setNFA(std::istream& is) {
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
void NFA<T>::_makeTransitionTable(std::istream &is) {
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
        while(s >> *t){
            auto zzz = _getPointer(t);
            if(zzz)addTransition(leftPoint, symbol, _getPointer(t));
        }
    }
}

template<class T>
T *NFA<T>::_getPointer(T *t) const {
    for(const auto i:states_){
        if((*i)==(*t)){
            return i;
        }
    }
    return nullptr;
}

template<class T>
bool NFA<T>::_isValidSymbol(const std::string &str) const {
    if(str==epsilon_){
        return true;
    }
    return sigma_.find(str) != sigma_.end();
}

/* public functions */

template<class T>
bool NFA<T>::addTransition(T *state1, std::string& str, T *state2) {
    if(sigma_.find(str) == sigma_.end() and str != epsilon_){
        return false;
    }
    if(states_.find(state1) == states_.end() or states_.find(state2) == states_.end()){
        return false;
    }
    delta_[state1][str].insert(state2);
    return true;
}

template<class STATE>
void NFA<STATE>::addState(STATE *state) {
    states_.insert(state);
}

template<class STATE>
void NFA<STATE>::addFinalState(STATE *state) {
    states_.insert(state);
    final_states_.insert(state);
}

/* operator functions */

template<class T>
std::istream &operator>>(std::istream &is, NFA<T> &nfa) {
    if(&is == &std::cin){
        std::cout << "Enter epsilon value: ";
        is >> nfa.epsilon_;
        is.ignore(reinterpret_cast<std::streamsize>(std::numeric_limits<std::streamsize>::max), '\n');
    }
    nfa._setNFA(is);
    if(&is == &std::cin){
        std::cout << "\n";
    }
    return is;
}

template<class T>
std::ostream &operator<<(std::ostream &os, NFA<T> &nfa) {
    using namespace prettyprint;
    prettyprint::make_default();
    os << "------------------- NFA -------------------------\n"
       << "Symbols:\n\t" << nfa.sigma_ << "\b\n"
       << "Starting State: " << *nfa.starting_state_ << "\n"
       << "States:\n\t" << nfa.states_ << "\b\n"
       << "Final states:\n\t" << nfa.final_states_ << "\b\n"
       << "Delta Transitions:\n" << nfa.delta_
       << "-------------------------------------------------\n";
    return os;
}

/* getter functions */

template<class T>
std::set<T *> NFA<T>::getEpsilonClosure(std::set<T *>& states){
    std::set<T*> ans;
    ans.clear();
    std::stack<T*> st;
    T* temp;
    for(auto i:states){
        ans.insert(i);
        st.push(i);
    }
    while(!st.empty()){
        temp = st.top();
        st.pop();
        for(T* i:delta_[temp][epsilon_]){
            if(ans.find(i) == ans.end()){
                ans.insert(i);
                st.push(i);
            }
        }
    }
    return ans;
}

template<class T>
std::set<T *> NFA<T>::transition(std::set<T *> states, const std::string& symbol) {
    std::set<T *> ans, temp;
    ans.clear();
    temp.clear();
    for(T* i: getEpsilonClosure(states)){
        temp = delta_[i][symbol];
        for(T* j: getEpsilonClosure(temp)){
            if(ans.find(j) == ans.end()){
                ans.insert(j);
            }
        }
    }
    return ans;
}

template<class T>
std::set<T *> NFA<T>::transition(T *state, const std::string &symbol) {
    return transition(std::set<T *>{state}, symbol);
}

template<class STATE>
const std::set<std::string> &NFA<STATE>::getSigma() const {
    return sigma_;
}

template<class STATE>
const std::set<STATE *> &NFA<STATE>::getStates() const {
    return states_;
}

template<class STATE>
STATE *NFA<STATE>::getStartingState() const {
    return starting_state_;
}

template<class STATE>
const std::set<STATE *> &NFA<STATE>::getFinalStates() const {
    return final_states_;
}

template<class STATE>
const std::string &NFA<STATE>::getEpsilon() const {
    return epsilon_;
}

#endif //COMPILER_ALGORITHMS_NFA_H
