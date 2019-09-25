//
// Created by hiren on 17/09/19.
//
#ifndef COMPILER_ALGORITHMS_PREDICTIVEPARSER_H
#define COMPILER_ALGORITHMS_PREDICTIVEPARSER_H

#include <stack>
#include <vector>
#include "topdowngrammar.h"
#include "../parser.h"

class PredictiveParser: public Parser<TopDownParsableGrammar>{
private:
    /* Predictive Parser P(G<Protected Member>, T)*/
    std::map<std::string, std::map<std::string, std::set<Prod*>>> table_;
    /* helper functions */
    void _fillTable();
    bool _checkString(std::vector<std::string> str, std::stack<std::string> st);
public:
    using Parser::Parser;
    /* override functions */
    bool checkString(std::vector<std::string>& str) override;
    void objectCreation() override;
    /* operator functions */
    friend std::ostream& operator <<(std::ostream& os, const PredictiveParser& parsingTable);
    std::map<std::string , std::set<Prod*>>& operator [](const std::string& str);
};
/* helper functions */
void PredictiveParser::_fillTable() {
    // initialize all entry
    for(const std::string& nt:gram_->getNonterminals()){
        for(const std::string& t:gram_->getTerminals()){
            table_[nt][t] = std::set<Prod*>{};
        }
        table_[nt]["$"] = std::set<Prod*>{};
    }
    for(const std::string& nt:gram_->getNonterminals()){
        // temp variable
        std::set<std::string> temp;
        bool check_follow;
        // check for each production
        for(Prod* p:gram_->getProdsForLeft(nt)){
            check_follow = false;
            temp.clear();
            temp = gram_->getFirst(p->getRight());
            if(temp.find(gram_->getEpsilon()) != temp.end()) {
                temp.erase(gram_->getEpsilon());
                check_follow = true;
            }
            for(auto &j:temp){
                table_[nt][j].insert(p);
            }
            if(check_follow){
                temp = gram_->getFollow(nt);
                for(auto &j:temp){
                    table_[nt][j].insert(p);
                }
            }
        }
    }
}
bool PredictiveParser::_checkString(std::vector<std::string> str, std::stack<std::string> st){
    while(!st.empty()){
        if(st.top() == str[0]){
            st.pop();
            str.erase(str.begin());
        }
        else if(gram_->isTerminal(st.top())){
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
                for(auto j = i->getRight().rbegin(); j != i->getRight().rend(); ++j){
                    temp.push(*j);
                }
                while(temp.top() == gram_->getEpsilon()){
                    temp.pop();
                }
                if(_checkString(str, temp)){
                    return true;
                }
            }
            return false;
        }
    }
    return true;
}
/* override functions */
bool PredictiveParser::checkString(std::vector<std::string> &str) {
    str.emplace_back("$");
    std::stack<std::string> st;
    st.push("$");
    st.push(gram_->getStartSymbol());
    return _checkString(str, st);
}
void PredictiveParser::objectCreation() {
    _fillTable();
}
/* operator functions */
std::ostream &operator<<(std::ostream &os, const PredictiveParser &parsingTable) {
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
std::map<std::string, std::set<Prod *>> &PredictiveParser::operator[](const std::string &str) {
    return table_[str];
}

#endif //COMPILER_ALGORITHMS_PREDICTIVEPARSER_H
