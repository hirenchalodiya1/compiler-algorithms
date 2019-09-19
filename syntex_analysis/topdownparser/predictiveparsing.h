#ifndef COMPILER_ALGORITHMS_PREDICTIVEPARSING_H
#define COMPILER_ALGORITHMS_PREDICTIVEPARSING_H

#include <stack>
#include <vector>
#include "firstfollow.h"

class PredictiveParsing{
private:
    PredictiveParsingTable* table_;
    bool _check_string(std::vector<std::string> str, std::stack<std::string> st);
public:
    explicit PredictiveParsing(std::istream& is, std::string e="?"){
        table_ = new PredictiveParsingTable(is, std::move(e));
    }
    bool check_string(std::vector<std::string>& str){
        str.emplace_back("$");
        std::stack<std::string> st;
        st.push("$");
        st.push(table_->grammar()->start_symbol());
        return _check_string(str, st);
    }
    void check_string(std::istream& is){
        std::string str, temp;
        std::vector<std::string> v;
        unsigned int i, n;
        while(std::getline(is, str)){
            v.clear();
            str.erase(str.find_last_not_of("\r\n")+1);
            i = 0;
            n = str.size();
            while(i < n && std::isspace(str[i])){
                ++i;
            }
            while(i < n){
                temp.clear();
                while(i < n && std::isalnum(str[i])){
                    temp.push_back(str[i]);
                    ++i;
                }
                if(!temp.empty()){
                    v.emplace_back(temp);
                }
                while(i < n && std::isspace(str[i])){
                    ++i;
                }
            }
            if(check_string((v))){
                std::cout << "Accepted\n";
            }
            else{
                std::cout << "Not Accepted\n";
            }
        }
    }
};

bool PredictiveParsing::_check_string(std::vector<std::string> str, std::stack<std::string> st){
    while(!st.empty()){
        if(st.top() == str[0]){
            st.pop();
            str.erase(str.begin());
        }
        else if(table_->grammar()->is_terminal(st.top())){
            return false;
        }
        else if((*table_)[st.top()][str[0]].empty()){
            return false;
        }
        else{
            auto top = st.top();
            st.pop();
            for(auto i:(*table_)[top][str[0]]){
                std::stack<std::string> temp = st;
                for(auto j = i->right().rbegin(); j != i->right().rend(); ++j){
                    temp.push(*j);
                }
                while(temp.top() == table_->grammar()->epsilon()){
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
