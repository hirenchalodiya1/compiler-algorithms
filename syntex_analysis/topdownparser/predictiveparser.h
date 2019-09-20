//
// Created by hiren on 17/09/19.
//
#ifndef COMPILER_ALGORITHMS_PREDICTIVEPARSER_H
#define COMPILER_ALGORITHMS_PREDICTIVEPARSER_H

#include <stack>
#include <vector>
#include "topdowngrammer.h"
#include "../parser.h"

class PredictiveParser: public Parser{
private:
    TopDownParsableGrammar* gram_;
    std::map<std::string, std::map<std::string, std::set<Prod*>>> table_;
    void _fill_table();
    bool _check_string(std::vector<std::string> str, std::stack<std::string> st);
public:
    explicit PredictiveParser(const std::string& e ="?"){
        gram_ = new TopDownParsableGrammar(e);
    }
    explicit PredictiveParser(std::istream& is,const std::string& e="?"){
        gram_ = new TopDownParsableGrammar(is, e);
        _fill_table();
    }
    bool check_string(std::vector<std::string>& str) override{
        str.emplace_back("$");
        std::stack<std::string> st;
        st.push("$");
        st.push(gram_->start_symbol());
        return _check_string(str, st);
    }
    friend std::ostream& operator <<(std::ostream& os, const PredictiveParser& parsingTable){
        using namespace prettyprint;
        prettyprint::line_start = "\t";
        prettyprint::line_end = "\n";
        for(auto& i:parsingTable.table_){
            for(auto& j:i.second){
                os << "Cell:[" << i.first << ", " << j.first << "]\n";
                if(j.second.empty()){
                    os << "\tNone\n";
                }
                else{
                    os << j.second;
                }
            }
        }
        prettyprint::make_default();
        return os;
    }
    friend std::istream& operator >>(std::istream& is, PredictiveParser& parser){
        is >> *parser.gram_;
        parser._fill_table();
        return is;
    }
    std::map<std::string , std::set<Prod*>>& operator [](const std::string& str){
        return table_[str];
    }
};
void PredictiveParser::_fill_table() {
    // initialize all entry
    for(const std::string& nt:gram_->nonterminals()){
        for(const std::string& t:gram_->terminals()){
            table_[nt][t] = std::set<Prod*>{};
        }
        table_[nt]["$"] = std::set<Prod*>{};
    }
    for(const std::string& nt:gram_->nonterminals()){
        // temp variable
        std::set<std::string> temp;
        bool check_follow;
        // check for each production
        for(Prod* p:gram_->get_rules_for_left(nt)){
            check_follow = false;
            temp.clear();
            temp = gram_->get_first(p->right());
            if(temp.find(gram_->epsilon()) != temp.end()) {
                temp.erase(gram_->epsilon());
                check_follow = true;
            }
            for(auto &j:temp){
                table_[nt][j].insert(p);
            }
            if(check_follow){
                temp = gram_->get_follow(nt);
                for(auto &j:temp){
                    table_[nt][j].insert(p);
                }
            }
        }
    }
}
bool PredictiveParser::_check_string(std::vector<std::string> str, std::stack<std::string> st){
    while(!st.empty()){
        if(st.top() == str[0]){
            st.pop();
            str.erase(str.begin());
        }
        else if(gram_->is_terminal(st.top())){
            return false;
        }
        else if(table_[st.top()][str[0]].empty()){
            return false;
        }
        else{
            auto top = st.top();
            st.pop();
            for(auto i:table_[top][str[0]]){
                std::stack<std::string> temp = st;
                for(auto j = i->right().rbegin(); j != i->right().rend(); ++j){
                    temp.push(*j);
                }
                while(temp.top() == gram_->epsilon()){
                    temp.pop();
                }
                if(_check_string(str, temp)){
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}

#endif //COMPILER_ALGORITHMS_PREDICTIVEPARSER_H
