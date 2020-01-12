
#ifndef COMPILER_ALGORITHMS_CREATE_H
#define COMPILER_ALGORITHMS_CREATE_H

#include <automatas/dfa.h>
#include <sstream>
#include <iostream>

DFA<std::string, std::string> createDFA(std::istream& is){
    DFA<std::string, std::string> dfa;

    // required variables
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
            dfa.addAlphabet(temp);
        }
    }
    std::string t;
    // get all states
    if(&is == &std::cin){
        std::cout << "Enter all states separated by space and then hit enter:\n";
    }
    temp_char = 't';
    while (temp_char != '\n'){
        is >> t;
        dfa.addState(t);
        temp_char = is.get();
    }
    // get starting state
    if(&is == &std::cin){
        std::cout << "Enter starting state and then hit enter: ";
    }
    is >> t;
    dfa.setStartingState(t);
    // final states
    if(&is == &std::cin){
        std::cout << "Enter all final states separated by space and then hit enter:\n";
    }
    temp_char = 't';
    while (temp_char != '\n'){
        is >> t;
        dfa.addFinalState(t);
        temp_char = is.get();
    }

    // make transition table
    if(&is == &std::cin){
        std::cout << "Enter all transitions in this format <STATE><SPACE>, <SYMBOL><SPACE> - <STATES SEPARATED BY SPACE AND HIT ENTER>:\n";
        std::cout << "Press CTRL + D (EOF) to end stream\n";
    }

    const char arrow = '-';
    const char delimiter= ',';

    std::string  symbol, left, right;
    while (std::getline(is, line)) {
        line.erase(line.find_last_not_of("\r\n") + 1);
        std::stringstream s(line);
        s >> left;
        if(!dfa.isValidState(left)){continue;}
        s >> temp_char;
        if(std::isspace(temp_char)){s >> temp_char;}
        if(temp_char == EOF){continue;}
        if(temp_char != delimiter){continue;}
        s >> symbol;
        s >> temp_char;
        if(!dfa.isValidAlphabet(symbol)){continue;}
        if(std::isspace(temp_char)){s >> temp_char;}
        if(temp_char == EOF){continue;}
        if(temp_char != arrow){continue;}
        s >> right;
        if(!dfa.isValidState(right))continue;
        dfa.addTransition(left, symbol, right);
    }
    return dfa;
}

#endif //COMPILER_ALGORITHMS_CREATE_H
