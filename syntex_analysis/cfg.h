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
    /* getter functions */
    const std::string &getLeft() const {
        return left_;
    }
    std::vector<std::string> &getRight(){
        return right_;
    }
    /* public functions */
    bool isExistOnRight(const std::string& str){
        return std::find(right_.begin(), right_.end(), str) != right_.end();
    }
    bool isEpsilonProduction(const std::string& epsilon){
        if(right_.size() == 1){
            return right_[0] == epsilon;
        }
        return false;
    }
    void addOnLeftOfRight(std::vector<std::string> st){
        st.insert(st.end(), right_.begin(), right_.end());
        right_ = st;
    }
    std::vector<std::string> getFromIthPosition(int i, const std::string& epsilon){
        std::vector<std::string> str{};
        for(unsigned long int j = i; j < right_.size(); j++){
            str.push_back(right_[j]);
        }
        if(str.empty()){
            str.push_back(epsilon);
        }
        return str;
    }
    std::vector<std::string> getTillIthPosition(int i, const std::string& epsilon){
        std::vector<std::string> str{};
        for(int j = 0; j < i; j++){
            str.push_back(right_[j]);
        }
        if(str.empty()){
            str.push_back(epsilon);
        }
        return str;
    }
    /* operator functions */
    friend std::ostream& operator <<(std::ostream& os, const Prod& p);
};
std::ostream &operator<<(std::ostream &os, const Prod &p) {
    os << p.left_<< " --> ";
    for(const std::string &i:p.right_){
        os << i << " ";
    }
    os << "\b";
    return os;
}

class Rule{
private:
    std::string left_;
    std::vector<std::vector<std::string>> right_;
    bool success_;

public:
    explicit Rule(std::string &str) {
        const char separator = '|';
        const std::string arrow = "--";
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
        if(!(success_ = str.substr(i, arrow.size()) == arrow)){
            return ;
        }
        i += arrow.size();
        while (i < n && std::isspace(str[i])){
            ++i;
        }
        // right side
        success_ = false;
        while(i < n){
            bool added = false;
            while(i < n && str[i] != separator){
                std::string symbol;
                while(i < n && !std::isspace(str[i]) && str[i] != separator) {
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
        left_ = (*prods.begin())->getLeft();
        for(Prod* p:prods){
            right_.emplace_back(p->getRight());
        }
    }
    explicit operator bool(){
        return success_;
    }
    /* getter functions */
    const std::string &getLeft() const {
        return left_;
    }
    const std::vector<std::vector<std::string>> &getRight() const {
        return right_;
    }
    /* operator functions */
    friend std::ostream& operator <<(std::ostream& os, const Rule& r);
};
std::ostream &operator<<(std::ostream &os, const Rule &r) {
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
    std::set<std::string> first_done;
    std::set<std::string> follow_done;
    bool empty_;

    /* define helper functions*/
    void _setGrammar(std::istream& is);
    void _convertRuleToProd();
    void _prepareFirst();
    void _prepareFollow();
public:
    explicit CFG(std::string e="?"):epsilon_(std::move(e)){empty_=true;}
    explicit CFG(std::istream& is,const std::string& e="?"){
        empty_ = true;
        epsilon_ = e;
        _setGrammar(is);
    }
    explicit operator bool(){
        return !empty_;
    }
    virtual void objectCreation(){ FirstFollow();}

    /* getter function */
    const std::set<std::string> &getTerminals() const;
    const std::set<std::string> &getNonterminals() const;
    const std::string &getStartSymbol() const;
    const std::string &getEpsilon() const;
     /* first follow function */
    void FirstFollow();
    std::set<std::string> getFirst(const std::vector<std::string>& prod);
    std::set<std::string> getFirst(const std::string& symbol);
    std::set<std::string> getFollow(const std::string& symbol);
    /* public functions */
    std::set<Prod*> getProdsForLeft(const std::string& str) const;
    std::set<Prod*> getProdsForRight(const std::string& str) const;
    bool isTerminal(const std::string& str) const;
    bool haveEpsilonProduction(const std::string& str);
    std::string generateNewSymbol(std::string str);
    /* operator functions */
    friend std::ostream& operator <<(std::ostream& os, const CFG& gram);
    friend std::istream& operator >>(std::istream& is, CFG& gram);
};
// declare helper functions
void CFG::_setGrammar(std::istream& is) {
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
    start_symbol_ = rules_[0]->getLeft();
    // get all nonterminal symbols
    for(Rule* r: rules_){
        nonterminals_.insert(r->getLeft());
        for(const std::vector<std::string>& i:r->getRight()){
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
    _convertRuleToProd();
    // create method for object creation
    objectCreation();
}
void CFG::_convertRuleToProd(){
    for(Rule *r:rules_){
        for(auto& right:r->getRight()){
            prods_.insert(new Prod(r->getLeft(), right));
        }
    }
}
void CFG::_prepareFirst() {
    for(auto& i:nonterminals_){
        getFirst(i);
    }
}
void CFG::_prepareFollow() {
    for(auto&i:nonterminals_){
        getFollow(i);
    }
}
/* first follow functions*/
void CFG::FirstFollow() {
    // prepare first of all terminal
    _prepareFirst();

    // prepare follow of all terminal
    follow_[start_symbol_].insert("$");
    _prepareFollow();
}
std::set<std::string> CFG::getFirst(const std::vector<std::string>& prod){
    if(prod.size() == 1 and prod[0] == epsilon_){
        return std::set<std::string>{epsilon_};
    }
    if(isTerminal(prod[0])){
        return std::set<std::string>{prod[0]};
    }
    std::set<std::string> ans, temp;
    bool ep = true;
    for(auto &i:prod){
        temp = getFirst(i);
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
std::set<std::string> CFG::getFirst(const std::string& symbol){
    // string end symbol or epsilon symbol
    if(symbol=="$" or symbol==epsilon_){
        return std::set<std::string>{symbol};
    }
    // condition 1 if terminal
    if(isTerminal(symbol)){
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
    std::set<std::string> ans, temp;
    for(Prod* p:getProdsForLeft(symbol)){
        temp = getFirst(p->getRight());
        for(auto &j:temp){
            ans.insert(j);
        }
    }
    // store in first for future purpose
    first_[symbol] = ans;
    return ans;
}
std::set<std::string> CFG::getFollow(const std::string& symbol){
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
    for(auto *p:getProdsForRight(symbol)){
        add_follow = false;
        // remove all symbols which are getLeft of current symbol
        for(std::vector<std::string> editable_p = p->getRight(); !editable_p.empty();){
            temp.clear();
            if(editable_p[0] == symbol){
                editable_p.erase(editable_p.begin());
                // if empty then find follow of first
                if(!editable_p.empty()){
                    temp = getFirst(editable_p);
                    if(temp.find(getEpsilon()) != temp.end()){
                        temp.erase(getEpsilon());
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
            temp = getFollow(p->getLeft());
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
/* getter functions */
const std::set<std::string> &CFG::getTerminals() const {
    return terminals_;
}
const std::set<std::string> &CFG::getNonterminals() const {
    return nonterminals_;
}
const std::string &CFG::getStartSymbol() const {
    return start_symbol_;
}
const std::string &CFG::getEpsilon() const {
    return epsilon_;
}
/* public functions */
std::set<Prod *> CFG::getProdsForLeft(const std::string &str) const {
    std::set<Prod*> prod;
    for(Prod* p:prods_){
        if(p->getLeft() == str){
            prod.insert(p);
        }
    }
    return prod;
}
std::set<Prod *> CFG::getProdsForRight(const std::string &str) const {
    std::set<Prod*> prod;
    for(Prod* p:prods_){
        if(p->isExistOnRight(str)){
            prod.insert(p);
        }
    }
    return prod;
}
bool CFG::isTerminal(const std::string &str) const {
    return terminals_.find(str) != terminals_.end();
}
bool CFG::haveEpsilonProduction(const std::string &str) {
    for(Prod* p:getProdsForLeft(str)){
        if(p->isEpsilonProduction(epsilon_)){
            return true;
        }
    }
    return false;
}
std::string CFG::generateNewSymbol(std::string str) {
    int digit=0;
    while (!str.empty()){
        if(std::isdigit(*str.rbegin())){
            digit = digit*10 + (*str.rbegin() - '0');
            str.pop_back();
        }
        else{
            break;
        }
    }
    std::string new_string = str + std::to_string(++digit);
    while(symbols_.find(new_string) != symbols_.end()){
        new_string = str + std::to_string(++digit);
    }
    symbols_.insert(new_string);
    nonterminals_.insert(new_string);
    return new_string;
}
/* operator function */
std::ostream &operator<<(std::ostream &os, const CFG &gram) {
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
std::istream &operator>>(std::istream &is, CFG &gram) {
    if(&is == &std::cin){
        std::cout << "Enter epsilon value: ";
        is >> gram.epsilon_;
        std::cout << "Press CTRL + D (EOF) to end stream\n";
    }
    gram._setGrammar(is);
    if(&is == &std::cin){
        std::cout << "\n";
    }
    return is;
}

#endif //COMPILER_ALGORITHMS_CFG_H