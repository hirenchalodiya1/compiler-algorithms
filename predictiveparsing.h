#ifndef COMPILER_ALGORITHMS_PREDICTIVEPARSING_H
#define COMPILER_ALGORITHMS_PREDICTIVEPARSING_H

#include <stack>
#include <vector>
#include "firstfollow.h"

class PredictiveParsing{
private:
    PredictiveParsingTable* table;
    bool _check_string(std::vector<std::string> str, std::stack<std::string> st);
public:
    explicit PredictiveParsing(std::istream& is, std::string e="?"){
        table = new PredictiveParsingTable(is, std::move(e));
    }
    bool check_string(std::vector<std::string>& str){
        str.emplace_back("$");
        std::stack<std::string> st;
        st.push("$");
        st.push(table->grammar()->start_symbol());
        return _check_string(str, st);
    }
};
bool PredictiveParsing::_check_string(std::vector<std::string> str, std::stack<std::string> st){
    while(!st.empty()){
        if(st.top() == str[0]){
            st.pop();
            str.erase(str.begin());
        }
        else if(table->grammar()->is_terminal(st.top())){
            return false;
        }
        else if((*table)[st.top()][str[0]].empty()){
            return false;
        }
        else{
            auto top = st.top();
            st.pop();
            for(auto i:(*table)[top][str[0]]){
                std::stack<std::string> temp = st;
                for(int j = i->right().size()-1; j >= 0; --j){
                    temp.push(i->right()[j]);
                }
                while(temp.top() == table->grammar()->epsilon()){
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
#endif //COMPILER_ALGORITHMS_PREDICTIVEPARSING_H
