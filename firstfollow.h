#ifndef COMPILER_ALGORITHMS_FIRSTFOLLOW_H
#define COMPILER_ALGORITHMS_FIRSTFOLLOW_H

#include <utility>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include "cfg.h"


class PredictiveParsingTable{
private:
    CFG* gram;
    std::map<std::string, std::set<std::string>> first_;
    std::map<std::string, std::set<std::string>> follow_;
    std::set<std::string> first_done;
    std::set<std::string> follow_done;

    void _fill_table();
    void _prepare_first();
    void _prepare_follow();
    std::set<std::string> get_first(const std::vector<std::string>& prod);
    std::set<std::string> get_first(const std::string& symbol);
    std::set<std::string> get_follow(const std::string& symbol);
public:
    std::map<std::string, std::map<std::string, std::set<Prod*>>> table;
    explicit  PredictiveParsingTable(std::istream& is, std::string e = "ep"){
        // make grammar
        gram = new CFG(is, std::move(e));
        // make table with no value
        for(const std::string& nt:gram->nonterminals()){
            for(const std::string& t:gram->terminals()){
                table[nt][t] = std::set<Prod*>{};
            }
            table[nt]["$"] = std::set<Prod*>{};
        }
        // add $ in follow of start symbol
        follow_[gram->start_symbol()].insert("$");
        // prepare all first of all nonterminal symbol
        _prepare_first();
        // prepare all follow
        _prepare_follow();
        // fill table
        _fill_table();
    }
    friend std::ostream& operator <<(std::ostream& os, const PredictiveParsingTable& parsingTable){
        for(auto& i:parsingTable.table){
            for(auto& j:i.second){
                os << "Cell:[" << i.first << ", " << j.first << "]\n";
                if(j.second.empty()){
                    os << "\tNone\n";
                }
                else{
                    for(auto& k:j.second){
                        os << "\t" << *k << "\n";
                    }
                }
            }
        }
        return os;
    }
    CFG* grammar(){
        return gram;
    }
    /*
    void printFirst(){
        for(auto &i:follow_){
            std::cout << i.first << " --- ";
            for(auto &j:i.second){
                std::cout << j << " ";
            }
            std::cout << "\n";
        }
    }*/
};
void PredictiveParsingTable::_prepare_first() {
    for(auto&i:gram->nonterminals()){
        get_first(i);
    }
}
void PredictiveParsingTable::_prepare_follow() {
    for(auto&i:gram->nonterminals()){
        get_first(i);
    }
}
void PredictiveParsingTable::_fill_table() {
    for(const std::string& nt:gram->nonterminals()){
        // temp variable
        std::set<std::string> temp;
        bool check_follow;
        // check for each production
        for(Prod* p:gram->get_rules_for_left(nt)){
            check_follow = false;
            temp.clear();
            temp = get_first(p->right());
            if(temp.find(gram->epsilon()) != temp.end()) {
                temp.erase(gram->epsilon());
                check_follow = true;
            }
            for(auto &j:temp){
                table[nt][j].insert(p);
            }
            if(check_follow){
                    temp = get_follow(nt);
                    for(auto &j:temp){
                        table[nt][j].insert(p);
                    }
            }
        }
    }
}
std::set<std::string> PredictiveParsingTable::get_first(const std::vector<std::string>& prod){
    if(prod.size() == 1 and prod[0] == gram->epsilon()){
        return std::set<std::string>{gram->epsilon()};
    }
    if(gram->is_terminal(prod[0])){
        return std::set<std::string>{prod[0]};
    }
    std::set<std::string> ans, temp;
    bool ep = true;
    for(auto &i:prod){
        temp = get_first(i);
        if(temp.find(gram->epsilon()) == temp.end()){
            for(auto &j:temp){
                ans.insert(j);
            }
            ep = false;
            break;
        }
        temp.erase(gram->epsilon());
        for(auto &j:temp){
            ans.insert(j);
        }
    }
    if(ep)ans.insert(gram->epsilon());
    return ans;
}
std::set<std::string> PredictiveParsingTable::get_first(const std::string& symbol){
    // condition 1 if terminal
    if(gram->is_terminal(symbol)){
        return std::set<std::string>{symbol};
    }
    // Recursion condition
    if(first_done.find(symbol) != first_done.end()){
        if(first_.find(symbol) != first_.end()){
            return first_[symbol];
        }
        else{
            return std::set<std::string>{};
        }
    }
    first_done.insert(symbol);

    // store ans
    std::set<std::string> ans;

    // condition 2
    for(Prod* i:gram->get_rules_for_left(symbol)){
        if(i->right().size() == 1 && i->right()[0] == gram->epsilon()){
            ans.insert(gram->epsilon());
        }
    }
    std::set<std::string> temp;
    for(Prod* p:gram->get_rules_for_left(symbol)){
        temp = get_first(p->right());
        for(auto &j:temp){
            ans.insert(j);
        }
    }
    // store in first for future purpose
    first_[symbol] = ans;
    return ans;
}
std::set<std::string> PredictiveParsingTable::get_follow(const std::string& symbol){
    // Recursion condition
    if(follow_done.find(symbol) != follow_done.end()){
        if(follow_.find(symbol) != follow_.end()){
            return follow_[symbol];
        }
        else{
            return std::set<std::string>{};
        }
    }
    follow_done.insert(symbol);
    // store ans
    std::set<std::string> ans, temp;
    bool add_follow;
    for(auto *p:gram->get_rules_for_right(symbol)){
        std::vector<std::string> editable_p = p->right();
        add_follow = false;
        // remove all symbols which are left of current symbol
        while(!editable_p.empty()){
            if(editable_p[0] == symbol){
                editable_p.erase(editable_p.begin());
                break;
            }
            editable_p.erase(editable_p.begin());
        }
        // if empty then find follow of first left() of production or right part contains epsilon
        if(!editable_p.empty()){
            temp = get_first(editable_p);
            if(temp.find(gram->epsilon()) != temp.end()){
                temp.erase(gram->epsilon());
                add_follow = true;
            }
        }
        else{
            add_follow = true;
        }
        for(auto &j:temp){
            ans.insert(j);
        }
        if(add_follow){
            temp = get_follow(p->left());
            for(auto &j:temp){
                ans.insert(j);
            }
        }
    }
    // store in first for future purpose
    for(auto& i:ans){
        follow_[symbol].insert(i);
    }
    return follow_[symbol];
}

#endif //COMPILER_ALGORITHMS_FIRSTFOLLOW_H
