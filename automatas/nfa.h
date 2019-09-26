//
// Created by hiren on 26/09/19.
//

#ifndef COMPILER_ALGORITHMS_NFA_H
#define COMPILER_ALGORITHMS_NFA_H

#include <string>
#include <set>
#include <map>
#include <iostream>

template <class STATE>
class NFA{
private:
    /* NFA Properties (Σ, Q, δ, q0, F) */
    std::set<std::string> sigma_;
    std::set<STATE*> states_;
    STATE* starting_state_;
    std::set<STATE*> final_states_;
    std::map<STATE*, std::map<std::string, std::set<STATE>>> delta_;

    std::string epsilon_;
    bool success_;

    void _setNFA(std::istream& is);
public:
    explicit NFA(std::string e="ep"):epsilon_(std::move(e)){
        success_ = false;
    };
    explicit NFA(std::istream&is, std::string e="ep"):epsilon_(std::move(e)){
        _setNFA(is);
    }
    explicit operator bool(){
        return success_;
    }
    /* operator function */
    friend std::istream&operator>>(std::istream& is, NFA<STATE>& nfa){
        if(&is == &std::cin){
            std::cout << "Enter epsilon value: ";
            is >> nfa.epsilon_;
            std::cout << "Press CTRL + D (EOF) to end stream\n";
        }
        nfa._setNFA(is);
        if(&is == &std::cin){
            std::cout << "\n";
        }
        return is;
    }
    friend std::ostream&operator<<(std::ostream& os, NFA<STATE>& nfa){
        os << *nfa.starting_state_ << "\n";
        return os;
    }
};
template <class T>
void NFA<T>::_setNFA(std::istream& is) {
    std::string line, temp;
    char temp_char;
    unsigned int i, n;
    // get all symbols
    if(&is == &std::cin){
        std::cout << "Enter all symbols separated by space and then hit enter: ";
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
    T t;
    // get all states
    if(&is == &std::cin){
        std::cout << "Enter all states separated by space and then hit enter: ";
    }
    temp_char = is.get();
    if(!(success_ = (temp_char != EOF))){
        return;;
    }
    while (temp_char != '\n'){
        is >> t;
        states_.emplace(&t);
        temp_char = is.get();
        if(!(success_ = (temp_char != EOF))){
            return;;
        }
    }
    if(!(success_ = !states_.empty())){
        return;
    }
    // get staring state
    if(&is == &std::cin){
        std::cout << "Enter starting state and then hit enter: ";
    }
    is >> t;
    starting_state_  = &t;
    if(!(success_ = (is.get() == '\n'))){
        return;
    }
    // final states
    if(&is == &std::cin){
        std::cout << "Enter all states separated by space and then hit enter: ";
    }
    temp_char = is.get();
    if(&is == &std::cin){
        std::cout << "Enter all final states separated by space and then hit enter: ";
    }
    while (is.get() != '\n'){
        is >> t;
        final_states_.emplace(&t);
        temp_char = is.get();
        if(!(success_ = (temp_char != EOF))){
            return;;
        }
    }
    if(!(success_ = !final_states_.empty())){
        return;
    }
}

#endif //COMPILER_ALGORITHMS_NFA_H
