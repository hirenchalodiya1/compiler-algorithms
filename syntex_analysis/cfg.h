//
// Created by hiren on 17/09/19.
//
#ifndef COMPILER_ALGORITHMS_CFG_H
#define COMPILER_ALGORITHMS_CFG_H

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


class Prod{
private:
    std::string left_;
    std::vector<std::string> right_;

public:
    explicit Prod(std::string  left, std::vector<std::string>  right):left_(std::move(left)), right_(std::move(right)){}
    std::string& left(){return left_;}
    std::vector<std::string>& right(){return right_;}
    bool is_exist_on_right(const std::string& str){
        return std::find(right_.begin(), right_.end(), str) != right_.end();
    }
    friend std::ostream& operator <<(std::ostream& os, const Prod& p){
        os << p.left_<< " --> ";
        for(const std::string &i:p.right_){
            os << i << " ";
        }
        return os;
    }
};

class Rule{
private:
    std::string left_;
    std::vector<std::vector<std::string>> right_;
    bool success_;

public:
    explicit Rule(std::string &str) {
        unsigned int i = 0;
        unsigned int n = str.size();
        while (i < n && std::isspace(str[i])) {
            ++i;
        }
        // left side of production rule
        while (i < n && std::isalnum(str[i])) {
            left_.push_back(str[i]);
            ++i;
        }
        if(!(success_ = !left_.empty())){
            return ;
        }

        while(i < n && std::isspace(str[i])){
            ++i;
        }
        // check separator
        if(!(success_ = str.substr(i, 2) == "--")){
            return ;
        }
        i += 2;
        while (i < n && std::isspace(str[i])){
            ++i;
        }
        // right side
        success_ = false;
        while(i < n){
            bool added = false;
            while(i < n && str[i] != '|'){
                std::string symbol;
                while(i < n && !std::isspace(str[i]) && str[i] != '|') {
                    symbol.push_back(str[i]);
                    ++i;
                }
                if(!symbol.empty()){
                    if(!added){
                        right_.emplace_back(std::vector<std::string>());
                        added = true;
                        success_ = true;
                    }
                    right_.back().push_back(symbol);
                }
                while(i < n && std::isspace(str[i])){
                    ++i;
                }
            }
            if(i < n){
                ++i;
            }
        }
    }
    explicit operator bool(){
        return success_;
    }
    const std::string& left(){
        return left_;
    }
    const std::vector<std::vector<std::string>>& right(){
        return right_;
    }
    friend std::ostream& operator <<(std::ostream& os, const Rule& r){
        os << r.left_ << " --> ";
        unsigned int i = 0;
        for(const std::vector<std::string>& j:r.right_){
            for(const std::string& k:j){
                os << k << " ";
            }
            if(i++ < r.right_.size() - 1){
                os << "| ";
            }
        }
        return os;
    }
};

namespace spaceprint{
    inline std::ostream& operator <<(std::ostream& os, const std::vector<std::string>& v){
        for(const std::string& i:v){
            os << i << ' ';
        }
        return os;
    }
    inline std::ostream& operator <<(std::ostream& os, const std::set<std::string>& s){
        for(const std::string& i:s){
            os << i << ' ';
        }
        return os;
    }
}

namespace cfg{
    inline std::ostream& operator <<(std::ostream& os, const std::vector<Prod*>& prods){
        for(Prod* p:prods){
            os << *p << '\n';
        }
        return os;
    }
    inline std::ostream& operator <<(std::ostream& os, const std::vector<Rule*>& rules){
        for(Rule* r:rules){
            os << *r << '\n';
        }
        return os;
    }
}

class CFG{
private:
    std::vector<Rule*> rules_;
    std::vector<Prod*> prods_;
    std::string start_symbol_;
    std::set<std::string> terminals_;
    std::set<std::string> nonterminals_;
    std::set<std::string> symbols_;
    std::string epsilon_;
public:
    explicit CFG(std::istream& is, std::string e="ep"){
        epsilon_ = std::move(e);
        std::string line;
        // get all rules
        while (std::getline(is, line)) {
            line.erase(line.find_last_not_of("\r\n") + 1);
            if (!line.empty()) {
                Rule *r = new Rule(line);
                if (*r) {
                    rules_.emplace_back(r);
                }
            }
        }
        // get start symbol
        start_symbol_ = rules_[0]->left();
        // get all nonterminal symbols
        for(Rule* r: rules_){
            nonterminals_.insert(r->left());
            for(const std::vector<std::string>& i:r->right()){
                for(const std::string& j:i){
                    symbols_.insert(j);
                }
            }
        }
        // get all terminal symbols
        for(const std::string& i:symbols_){
            if(nonterminals_.find(i) == nonterminals_.end()){
                terminals_.insert(i);
            }
        }
        // remove epsilon from terminal symbol
        if(terminals_.find(epsilon_) != terminals_.end()){
            terminals_.erase(epsilon_);
        }
        // convert rule to productions
        _convert_rule_to_prod();
    }
    void _convert_rule_to_prod(){
        for(Rule *r:rules_){
            for(auto& right:r->right()){
                prods_.emplace_back(new Prod(r->left(), right));
            }
        }
    }
    std::set<std::string> terminals() const{
        return terminals_;
    }
    std::set<std::string> nonterminals() const{
        return nonterminals_;
    }
    std::string start_symbol() const{
        return start_symbol_;
    }
    std::string epsilon() const{
        return epsilon_;
    }

    //get all of the rules staring with str symbol
    std::vector<Prod*> get_rules_for_left(const std::string& str) const {
        std::vector<Prod*> prod;
        for(Prod* p:prods_){
            if(p->left() == str){
                    prod.push_back(p);
            }
        }
        return prod;
    }
    // get all production which contains string at right side
    std::vector<Prod*> get_rules_for_right(const std::string& str) const{
        std::vector<Prod*> prod;
        for(Prod* p:prods_){
            if(p->is_exist_on_right(str)){
                prod.emplace_back(p);
            }
        }
        return prod;
    }
    bool is_terminal(const std::string& str) const{
        return terminals_.find(str) != terminals_.end();
    }
    // pretty print
    friend std::ostream& operator <<(std::ostream& os, const CFG& gram){
        using namespace cfg;
        using namespace spaceprint;
        os << "----------------------- Grammar -----------------------\n"
        << "Start Symbol --> " << gram.start_symbol_ << "\n\n"
        << "Non-Terminal Symbols:\n\t" << gram.nonterminals_ << "\n\n"
        << "Terminal Symbols:\n\t" << gram.terminals_ << "\n\n"
        << "Rules:\n" << gram.rules_ << "\n"
        << "Productions:\n" << gram.prods_
        << "-------------------------------------------------------\n";
        return os;
    }
};

#endif //COMPILER_ALGORITHMS_CFG_H