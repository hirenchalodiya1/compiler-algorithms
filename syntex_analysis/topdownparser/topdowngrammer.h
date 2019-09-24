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
    /* helper function definition */
    /* remove left recursion */
    std::string _removeDirectLeftRecursion(std::string& st);
    void _replaceJInI(std::string &st1, std::string& st2);
    void _removeLeftRecursion();
    /* remove left factoring */
    void _removeLeftFactoring(const std::string& str);
    void _removeLeftFactoring();
    /* update rule according to production */
    void _convertProdToRule();
public:
    explicit TopDownParsableGrammar(const std::string& e="ep"):CFG(e){}
    explicit TopDownParsableGrammar(std::istream& is,const std::string& e="ep"):CFG(is, e){}
    void objectCreation() override {
        _removeLeftRecursion();
        _removeLeftFactoring();
        _convertProdToRule();
        FirstFollow();
    }
};
/* update rule to production*/
void TopDownParsableGrammar::_convertProdToRule() {
    rules_.clear();
    for(auto& nt:nonterminals_){
        Rule* temp = new Rule(getProdsForLeft(nt));
        if(*temp)rules_.emplace_back(temp);
    }
}
/* remove left recursion */
void TopDownParsableGrammar::_removeLeftRecursion() {
    // arrange non terminal in some order not necessary started with start symbol
    std::vector<std::string> nt;
    nt.push_back(start_symbol_);
    for(auto &i:nonterminals_){
        if(i != start_symbol_)nt.push_back(i);
    }
    // checking for all possible indirect recursion
    for(unsigned long int i = 0; i < nt.size(); i++){
        for(unsigned long int j = 0; j < i; j++){
            _replaceJInI(nt[i], nt[j]);
        }
        auto temp = _removeDirectLeftRecursion(nt[i]);
        if(!temp.empty()){
            nt.push_back(temp);
        }
    }
}
void TopDownParsableGrammar::_replaceJInI(std::string &st1, std::string& st2){
    Prod* temp;
    Prod* new_temp;
    auto st2_prod = getProdsForLeft(st2);
    for(Prod *p:getProdsForLeft(st1)){
        if(p->getRight()[0] == st2 and p->getRight().size() == 1){
            prods_.erase(p);
            for(Prod* j:st2_prod){
                prods_.insert(new Prod(st1, j->getRight()));
            }
        }
        else if(p->getRight()[0] == st2){
            prods_.erase(p);
            temp = p;
            temp->getRight().erase(temp->getRight().begin());
            for(Prod* k:st2_prod){
                if(k->isEpsilonProduction(epsilon_)){
                    continue;
                }
                else{
                    new_temp = new Prod(st1, temp->getRight());
                    new_temp->addOnLeftOfRight(k->getRight());
                    prods_.insert(new_temp);
                }
            }
        }
    }
}
std::string TopDownParsableGrammar::_removeDirectLeftRecursion(std::string& st){
    bool b = true;
    auto st_rule = getProdsForLeft(st);
     for(Prod* p:st_rule){
         if(p->getRight()[0] == st){
             b = false;
             break;
         }
     }
     if(b)return "";
    // new added symbols
    std::string new_st = generateNewSymbol(st);
    // check for all production with getLeft
    for(Prod* p:st_rule){
        if(p->getRight()[0] == st){
            prods_.erase(p);
            auto temp = p->getRight();
            temp.erase(temp.begin());
            temp.push_back(new_st);
            prods_.insert(new Prod(new_st, temp));
        }
        else{
            if(p->isEpsilonProduction(epsilon_)){
                p->getRight().push_back(new_st);
                p->getRight().erase(p->getRight().begin());
            }
            else{
                p->getRight().push_back(new_st);
            }
        }
    }
    prods_.insert(new Prod(new_st, std::vector<std::string>{epsilon_}));
    return new_st;
}
/* remove left factoring */
void TopDownParsableGrammar::_removeLeftFactoring(const std::string &str) {
    auto temp = getProdsForLeft(str);
    // map all production to according to start symbol of each production
    std::map<std::string, std::set<Prod*>> mapping;
    for(Prod* i:temp){
        if(!i->isEpsilonProduction(epsilon_)){
            mapping[i->getRight()[0]].insert(i);
        }
    }
    // remove all single entries
    auto it = mapping.cbegin();
    while(it != mapping.cend()){
        if(it->second.size() == 1){
            mapping.erase(it++);
        }
        else{
            it++;
        }
    }
    // if map entry end recursion
    if(mapping.empty()){
        return;
    }
    // remove left factoring
    auto iter = mapping.begin();
    std::vector<std::string> cur_key;
    bool increase;
    int count;
    std::string temp_;

    while(iter != mapping.end()){
        cur_key.clear();
        cur_key.push_back(iter->first);
        increase = true;
        count = 1;
        temp_.clear();

        while(increase){
            if((*iter->second.begin())->getRight().size() > count) {
                temp_ = (*iter->second.begin())->getRight()[count];
                for (auto j: iter->second) {
                    if (j->getRight().size() <= count or j->getRight()[count] != temp_){
                        increase = false;
                        break;
                    }
                }
                if(increase){cur_key.emplace_back(temp_);count++;}
            }
            else{
                increase = false;
            }
        }
        // change in current productions and remove it from prods_
        std::string new_str = generateNewSymbol(str);
        for(auto j:iter->second){
            prods_.insert(new Prod(new_str, j->getFromIthPosition(count, epsilon_)));
            prods_.erase(j);
        }
        // add new production instead
        auto add = (*iter->second.begin())->getTillIthPosition(count, epsilon_);
        add.push_back(new_str);
        prods_.insert(new Prod(str, add));
        // call recursive
        _removeLeftFactoring(new_str);
        // next iterator
        iter++;
    }
}
void TopDownParsableGrammar::_removeLeftFactoring() {
    std::vector<std::string> nt;
    nt.push_back(start_symbol_);
    for(auto &i:nonterminals_){
        if(i != start_symbol_)nt.push_back(i);
    }
    for(const std::string& i:nt){
        _removeLeftFactoring(i);
    }
}
#endif //COMPILER_ALGORITHMS_TOPDOWNGRAMMAR_H
