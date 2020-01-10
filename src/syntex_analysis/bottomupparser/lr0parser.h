//
// Created by hiren on 26/09/19.
//

#ifndef COMPILER_ALGORITHMS_LR0PARSER_H
#define COMPILER_ALGORITHMS_LR0PARSER_H

#include "../parser.h"
#include "augmentedgrammar.h"

class Item{
private:
    Prod* prod_;
    int pos_;
public:
    explicit Item(Prod* prod, int i=0):prod_(prod), pos_(i){};
};

class LR0Parser: public Parser<AugmentedGrammar>{
private:
    std::set<Item*> _getClosure(Item* item);
public:
    using Parser::Parser;
    /* override functions */
    void objectCreation() override {}
    bool checkString(std::vector<std::string> &str) override {return false;}
};
std::set<Item*> LR0Parser::_getClosure(Item* item){
    return std::set<Item*>{};
}

#endif //COMPILER_ALGORITHMS_LR0PARSER_H
