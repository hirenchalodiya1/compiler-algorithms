//
// Created by hiren on 21/09/19.
//

#ifndef COMPILER_ALGORITHMS_PARSER_H
#define COMPILER_ALGORITHMS_PARSER_H

#include <iostream>
#include <vector>

class Parser{
public:
    virtual bool check_string(std::vector<std::string>& str)=0;
    void check_file(std::istream& is){
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
            if(check_string(v)){
                std::cout << "Accepted\n";
            }
            else{
                std::cout << "Not Accepted\n";
            }
        }
    }
};


#endif //COMPILER_ALGORITHMS_PARSER_H
