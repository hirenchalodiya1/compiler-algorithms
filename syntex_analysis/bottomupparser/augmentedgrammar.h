//
// Created by hiren on 26/09/19.
//

#ifndef COMPILER_ALGORITHMS_AUGMENTEDGRAMMAR_H
#define COMPILER_ALGORITHMS_AUGMENTEDGRAMMAR_H

#include "../cfg.h"

class AugmentedGrammar : public CFG{
public:
    using CFG::CFG;
    /* override functions */
    void objectCreation() override {
        // generate new start symbol
        auto new_str = generateNewSymbol(start_symbol_);
        // add initial production
        prods_.insert(new Prod(new_str, std::vector<std::string>{start_symbol_}));
        // make new_str aas start symbol
        start_symbol_ = new_str;
        _convertProdToRule();
    }
};
#endif //COMPILER_ALGORITHMS_AUGMENTEDGRAMMAR_H
