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
    void _remove_direct_left_recursion(std::string& st);
    void _replace_j_in_i(std::string &st1, std::string& st2);
    void _remove_left_recursion();
    void _remove_left_factoring();
    void _convert_prod_to_rule();
public:
    explicit TopDownParsableGrammar(const std::string& e="ep"):CFG(e){}
    explicit TopDownParsableGrammar(std::istream& is,const std::string& e="ep"):CFG(is, e){}
    void object_creation() override {
        _remove_left_recursion();
        _remove_left_factoring();
        _convert_prod_to_rule();
        first_follow();
    }
};
void TopDownParsableGrammar::_convert_prod_to_rule() {
    rules_.clear();
    for(auto& nt:nonterminals_){
        Rule* temp = new Rule(get_rules_for_left(nt));
        if(*temp)rules_.emplace_back(temp);
    }
}
void TopDownParsableGrammar::_remove_left_recursion() {
    // arrange non terminal in some order not necessary started with start symbol
    std::vector<std::string> nt;
    nt.push_back(start_symbol_);
    for(auto &i:nonterminals_){
        if(i != start_symbol_)nt.push_back(i);
    }
    // checking for all possible indirect recursion
    for(unsigned long int i = 0; i < nt.size(); i++){
        for(unsigned long int j = 0; j < i; j++){
            _replace_j_in_i(nt[i], nt[j]);
        }
        _remove_direct_left_recursion(nt[i]);
    }
}
void TopDownParsableGrammar::_replace_j_in_i(std::string &st1, std::string& st2){
    Prod* temp;
    Prod* new_temp;
    auto st2_prod = get_rules_for_left(st2);
    for(Prod *p:get_rules_for_left(st1)){
        if(p->right()[0] == st2  and p->right().size() == 1){
            prods_.erase(p);
            for(Prod* j:st2_prod){
                prods_.insert(new Prod(st1, j->right()));
            }
        }
        else if(p->right()[0] == st2){
            prods_.erase(p);
            temp = p;
            temp->right().erase(temp->right().begin());
            for(Prod* k:st2_prod){
                if(k->right().size() == 1 and k->right()[0] == epsilon_){
                    continue;
                }
                else{
                    new_temp = new Prod(st1, temp->right());
                    new_temp->add_on_left(k->right());
                    prods_.insert(new_temp);
                }
            }
        }
    }
}
void TopDownParsableGrammar::_remove_direct_left_recursion(std::string& st){
    bool b = true;
    auto st_rule = get_rules_for_left(st);
     for(Prod* p:st_rule){
         if(p->right()[0]==st){
             b = false;
             break;
         }
     }
     if(b)return;
    // new added symbols
    std::string new_st = st + "1";
    while(nonterminals_.find(new_st)!=nonterminals_.end()){
        new_st.push_back('1');
    }
    nonterminals_.insert(new_st);
    // check for all production with left
    for(Prod* p:st_rule){
        if(p->right()[0]==st){
            prods_.erase(p);
            auto temp = p->right();
            temp.erase(temp.begin());
            temp.push_back(new_st);
            prods_.insert(new Prod(new_st, temp));
        }
        else{
            if(p->right().size() == 1 and p->right()[0] == epsilon_){
                p->right().push_back(new_st);
                p->right().erase(p->right().begin());
            }
            else{
                p->right().push_back(new_st);
            }
        }
    }
    prods_.insert(new Prod(new_st, std::vector<std::string>{epsilon_}));
}
void TopDownParsableGrammar::_remove_left_factoring() {
    // TODO: remove left factoring code
    for(auto nt:nonterminals_){
        /**/
    }
}
#endif //COMPILER_ALGORITHMS_TOPDOWNGRAMMAR_H
