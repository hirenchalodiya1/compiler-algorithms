//
// Created by hiren on 19/09/19.
//

#ifndef COMPILER_ALGORITHMS_TOPDOWNGRAMMAR_H
#define COMPILER_ALGORITHMS_TOPDOWNGRAMMAR_H

#include <iostream>
#include <string>
#include "../cfg.h"

class TopDownParsableGrammar : public CFG{
private:
    void _remove_left_recursion();
    void _remove_left_factoring();
public:
    explicit TopDownParsableGrammar(std::istream& is, std::string e="ep"):CFG(is, std::move(e)){
        _remove_left_recursion();
        _remove_left_factoring();
    }
};
void TopDownParsableGrammar::_remove_left_recursion() {

}
void TopDownParsableGrammar::_remove_left_factoring() {

}
#endif //COMPILER_ALGORITHMS_TOPDOWNGRAMMAR_H
