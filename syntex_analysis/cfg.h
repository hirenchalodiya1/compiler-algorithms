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
#include <map>

class Prod{
private:
    std::string left_;
    std::vector<std::string> right_;
public:
    explicit Prod(std::string left, std::vector<std::string> right):left_(std::move(left)), right_(std::move(right)){}
    std::string& left(){return left_;}
    std::vector<std::string>& right(){return right_;}
    bool is_exist_on_right(const std::string& str){
        return std::find(right_.begin(), right_.end(), str) != right_.end();
    }
    void add_on_left(std::vector<std::string> st){
        st.insert(st.end(), right_.begin(), right_.end());
        right_ = st;
    }
    friend std::ostream& operator <<(std::ostream& os, const Prod& p){
        os << p.left_<< " --> ";
        for(const std::string &i:p.right_){
            os << i << " ";
        }
        os << "\b";
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
    explicit Rule(const std::set<Prod*>& prods){
        if(prods.empty()){
            success_ = false;
            return;
        }
        success_ = true;
        left_ = (*prods.begin())->left();
        for(Prod* p:prods){
            right_.emplace_back(p->right());
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
        os << "\b";
        return os;
    }
};

namespace prettyprint{
    // TODO: make_default function should call automatic each time we use namespace
    std::string line_start="";
    std::string line_end=" ";
    void make_default(){
        line_start = "";
        line_end = " ";
    }
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::vector<T>& s){
        for(const auto& i:s){
            os << line_start << i << line_end;
        }
        return os;
    }
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::vector<T*>& s){
        for(auto *i:s){
            os << line_start << *i << line_end;
        }
        return os;
    }
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::set<T>& s){
        for(const auto& i:s){
            os << line_start << i << line_end;
        }
        return os;
    }
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::set<T*>& s){
        for(auto *i:s){
            os << line_start << *i << line_end;
        }
        return os;
    }
}

namespace cfg{
    inline std::ostream& operator <<(std::ostream& os, const std::set<Prod*>& prods){
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
    inline std::ostream& operator <<(std::ostream& os, const std::map<std::string, std::set<std::string>>& m){
        for(const auto & i : m){
            os << i.first << " --> "; prettyprint::operator<<(os,  i.second) << "\n";
        }
        return os;
    }
}

class CFG{
protected:
    /* Grammar CFG(V, T, P, S) */
    std::set<std::string> terminals_;
    std::set<std::string> nonterminals_;
    std::vector<Rule*> rules_;
    std::string start_symbol_;
    /* Additional Information */
    std::set<std::string> symbols_;
    std::set<Prod*> prods_;
    std::string epsilon_;
private:
    std::map<std::string, std::set<std::string>> first_;
    std::map<std::string, std::set<std::string>> follow_;
    /* helper data structures*/
    std::set<std::string> first_done;
    std::set<std::string> follow_done;
    bool empty_;

    /* define helper functions*/
    // convert rules to productions
    void _set_grammar(std::istream& is);
    void _convert_rule_to_prod();
    void _prepare_first();
    void _prepare_follow();
public:
    explicit CFG(std::string e="?"):epsilon_(std::move(e)){empty_=true;}
    explicit CFG(std::istream& is,const std::string& e="?"){
        empty_ = true;
        epsilon_ = e;
        _set_grammar(is);
    }
    explicit operator bool(){
        return !empty_;
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
    virtual void object_creation(){first_follow();}
    void first_follow(){
        // prepare first of all terminal
        _prepare_first();

        // prepare follow of all terminal
        follow_[start_symbol_].insert("$");
        _prepare_follow();
    }

    /* define public functions */
    std::set<std::string> get_first(const std::vector<std::string>& prod);
    std::set<std::string> get_first(const std::string& symbol);
    std::set<std::string> get_follow(const std::string& symbol);

    /* define and declare public functions */
    // get all of the rules staring with str symbol
    std::set<Prod*> get_rules_for_left(const std::string& str) const{
        std::set<Prod*> prod;
        for(Prod* p:prods_){
            if(p->left() == str){
                    prod.insert(p);
            }
        }
        return prod;
    }
    // get all production which contains string at right side
    std::set<Prod*> get_rules_for_right(const std::string& str) const{
        std::set<Prod*> prod;
        for(Prod* p:prods_){
            if(p->is_exist_on_right(str)){
                prod.insert(p);
            }
        }
        return prod;
    }
    bool is_terminal(const std::string& str) const{
        return terminals_.find(str) != terminals_.end();
    }
    bool have_epsilon_prod(const std::string& str){
        for(Prod* p:get_rules_for_left(str)){
            if(p->right().size() == 1 and p->right()[0] == epsilon_){
                return true;
            }
        }
        return false;
    }
    friend std::ostream& operator <<(std::ostream& os, const CFG& gram){
        using namespace cfg;
        using namespace prettyprint;
        os << "----------------------- Grammar -----------------------\n"
        << "Start Symbol --> " << gram.start_symbol_ << "\n\n"
        << "Non-Terminal Symbols:\n\t" << gram.nonterminals_ << "\n\n"
        << "Terminal Symbols:\n\t" << gram.terminals_ << "\n\n"
        << "Rules:\n" << gram.rules_ << "\n"
        << "Productions:\n" << gram.prods_ << "\n"
        << "Firsts:\n" << gram.first_ << "\n"
        << "Follows:\n" << gram.follow_
        << "-------------------------------------------------------\n";
        return os;
    }
    friend std::istream& operator >>(std::istream& is, CFG& gram){
        if(&is == &std::cin){
            std::cout << "Enter epsilon value: ";
            is >> gram.epsilon_;
            std::cout << "Press CTRL + D (EOF) to end stream\n";
        }
        gram._set_grammar(is);
        if(&is == &std::cin){
            std::cout << "\n";
        }
        return is;
    }
};
// declare helper functions
void CFG::_set_grammar(std::istream& is) {
    std::string line;
    // get all rules
    while (std::getline(is, line)) {
        line.erase(line.find_last_not_of("\r\n") + 1);
        if (!line.empty()) {
            Rule *r = new Rule(line);
            if (*r) {
                empty_ = false;
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
    // create method for object creation
    object_creation();
}
void CFG::_convert_rule_to_prod(){
    for(Rule *r:rules_){
        for(auto& right:r->right()){
            prods_.insert(new Prod(r->left(), right));
        }
    }
}
void CFG::_prepare_first() {
    for(auto& i:nonterminals_){
        get_first(i);
    }
}
void CFG::_prepare_follow() {
    for(auto&i:nonterminals_){
        get_follow(i);
    }
}
// declare public functions
std::set<std::string> CFG::get_first(const std::vector<std::string>& prod){
    if(prod.size() == 1 and prod[0] == epsilon_){
        return std::set<std::string>{epsilon_};
    }
    if(is_terminal(prod[0])){
        return std::set<std::string>{prod[0]};
    }
    std::set<std::string> ans, temp;
    bool ep = true;
    for(auto &i:prod){
        temp = get_first(i);
        if(temp.find(epsilon_) == temp.end()){
            for(auto &j:temp){
                ans.insert(j);
            }
            ep = false;
            break;
        }
        temp.erase(epsilon_);
        for(auto &j:temp){
            ans.insert(j);
        }
    }
    if(ep)ans.insert(epsilon_);
    return ans;
}
std::set<std::string> CFG::get_first(const std::string& symbol){
    // string end symbol or epsilon symbol
    if(symbol=="$" or symbol==epsilon_){
        return std::set<std::string>{symbol};
    }
    // condition 1 if terminal
    if(is_terminal(symbol)){
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
    for(Prod* i:get_rules_for_left(symbol)){
        if(i->right().size() == 1 && i->right()[0] == epsilon_){
            ans.insert(epsilon_);
        }
    }
    std::set<std::string> temp;
    for(Prod* p:get_rules_for_left(symbol)){
        temp = get_first(p->right());
        for(auto &j:temp){
            ans.insert(j);
        }
    }
    // store in first for future purpose
    first_[symbol] = ans;
    return ans;
}
std::set<std::string> CFG::get_follow(const std::string& symbol){
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
    for(auto *p:get_rules_for_right(symbol)){
        add_follow = false;
        // remove all symbols which are left of current symbol
        for(std::vector<std::string> editable_p = p->right();!editable_p.empty();){
            temp.clear();
            if(editable_p[0] == symbol){
                editable_p.erase(editable_p.begin());
                // if empty then find follow of first left() of production or right part contains epsilon
                if(!editable_p.empty()){
                    temp = get_first(editable_p);
                    if(temp.find(epsilon()) != temp.end()){
                        temp.erase(epsilon());
                        add_follow = true;
                    }
                }
                else{add_follow=true;}
                for(auto &j:temp){
                    ans.insert(j);
                }
            }
            else{
                editable_p.erase(editable_p.begin());
            }
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

#endif //COMPILER_ALGORITHMS_CFG_H