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
    return t and str.empty();
}
/* helper functions */
bool RecursiveParser::_checkString(std::vector<std::string>& str, const std::string& symbol) {
//    std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> check_;
    bool is_true;
    for(Prod* p: gram_->getProdsForLeft(symbol)){
        is_true = false;
        auto temp = str;
        for(auto& k:p->getRight()){
            if(k == gram_->getEpsilon()){
                is_true = true;
                continue;
            }
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
                if(temp.empty() and gram_->haveEpsilonProduction(k)){
                    is_true = true;
                    continue;
                }
                is_true = _checkString(temp, k);
            }
        }
        if(is_true){str = temp; return is_true;}
    }
    return false;
}

#endif //COMPILER_ALGORITHMS_RECURSIVEPARSER_H
