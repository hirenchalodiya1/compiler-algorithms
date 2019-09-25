//
// Created by hiren on 21/09/19.
//

#ifndef COMPILER_ALGORITHMS_RECURSIVEPARSER_H
#define COMPILER_ALGORITHMS_RECURSIVEPARSER_H

#include "topdowngrammer.h"
#include "../parser.h"

class RecursiveParser:public Parser<TopDownParsableGrammar>{
private:
    /* helper functions */
    bool _checkString(std::vector<std::string>& str, const std::string& symbol);
public:
    using Parser::Parser;
    /* override functions */
    bool checkString(std::vector<std::string>& str) override;
};
/* override functions */
bool RecursiveParser::checkString(std::vector<std::string> &str) {
    auto t = _checkString(str, gram_->getStartSymbol());
    auto& temp = gram_->getTerminals();
    for(auto&i:str){
        if(temp.find(i) == temp.end()){
            return false;
        }
    }
    return t and str.empty();
}
/* helper functions */
bool RecursiveParser::_checkString(std::vector<std::string>& str, const std::string& symbol) {
    bool is_true;
    for(Prod* p: gram_->getProdsForLeft(symbol)){
        if(p->isEpsilonProduction(gram_->getEpsilon())){
            continue;
        }
        is_true = false;
        auto temp = str;
        for(auto& k:p->getRight()){
            if(gram_->isTerminal(k)){
                if(!temp.empty() and temp[0] == k){
                    is_true = true;
                    temp.erase(temp.begin());
                }
                else{
                    is_true = false;
                    break;
                }
            }
            else{
                if(temp.empty()){
                    if(gram_->canGenerateEpsilonProduction(k)){
                        is_true = true;
                    }
                    else{
                        is_true = false;
                        break;
                    }
                }
                else{
                    is_true = _checkString(temp, k);
                }
            }
        }
        if(is_true){str = temp; return is_true;}
    }
    return false;
}

#endif //COMPILER_ALGORITHMS_RECURSIVEPARSER_H
