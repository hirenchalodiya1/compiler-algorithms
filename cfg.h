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

class Rule{
private:
    // left_ part of production rule
    // right_ part of production rule
    // success valid production rule
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
//    size_t size(unsigned int right) const {
//        return right_[right].size();
//    }
    explicit operator bool(){
        return success_;
    }
    const std::string& left() const{
        return left_;
    }
    const std::vector<std::vector<std::string>>& right() const{
        return right_;
    }
    friend std::ostream& operator <<(std::ostream& os, const Rule& r){
        os << r.left_ << " -- ";
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

class CFG{
private:
    std::vector<Rule*> rules_;
    std::string start_symbol_;
    std::vector<std::string> terminals_;
    std::set<std::string> nonterminals_;
    std::set<std::string> symbols_;
public:
    explicit CFG(std::istream& is){
        std::string line;
        while (std::getline(is, line)) {
            line.erase(line.find_last_not_of("\r\n") + 1);
            if (!line.empty()) {
                Rule *r = new Rule(line);
                if (r) {
                    rules_.emplace_back(r);
                }
            }
        }
        start_symbol_ = rules_[0]->left();
        for(Rule* r: rules_){
            nonterminals_.insert(r->left());
            for(const std::vector<std::string>& i:r->right()){
                for(const std::string& j:i){
                    symbols_.insert(j);
                }
            }
        }
        for(const std::string& i:symbols_){
            if(nonterminals_.find(i) == nonterminals_.end()){
                terminals_.push_back(i);
            }
        }
    }

    std::vector<Rule*> rules() const{
        return rules_;
    }
    std::vector<std::string> terminals() const{
        return terminals_;
    }
    std::vector<std::string> nonterminals() const{
        std::vector<std::string> vec;
        for(const auto& i:nonterminals_){
            vec.emplace_back(i);
        }
        return vec;
    }
    std::string start_symbol() const{
        return start_symbol_;
    }

    //get all of the rules staring with str symbol
    template <class Output>
    Output get_rules_for_left(const std::string& str, Output it) const {
        for(Rule* r:rules_){
            if(r->left() == str){
                *it++ = r;
            }
        }
        return it;
    }

    bool is_terminal(const std::string& str) const{
        return std::find(terminals_.begin(), terminals_.end(), str) != terminals_.end();
    }

    // print
    friend std::ostream& operator <<(std::ostream& os, const CFG& gram){
        for(Rule* r:gram.rules_){
            os << *r << '\n';
        }
        return os;
    }
};
#endif //COMPILER_ALGORITHMS_CFG_H