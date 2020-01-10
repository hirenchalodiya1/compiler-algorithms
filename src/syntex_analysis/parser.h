//
// Created by hiren on 21/09/19.
//

#ifndef COMPILER_ALGORITHMS_PARSER_H
#define COMPILER_ALGORITHMS_PARSER_H

#include <iostream>
#include <vector>

template <class T>
class Parser{
protected:
    T* gram_;
private:
    bool success_;
public:
    explicit Parser(const std::string& e ="?"){
        gram_ = new T(e);
        success_ = false;
    }
    explicit Parser(std::istream& is,const std::string& e="?"){
        gram_ = new T(is, e);
        success_ = (bool)*gram_;
        objectCreation();
    }
    explicit operator bool(){
        return success_;
    }
    /* virtual functions */
    virtual bool checkString(std::vector<std::string>& str)=0;
    virtual void objectCreation()=0;
    /* public functions */
    T *getGram() const {
        return gram_;
    }
    void checkFile(std::istream& is){
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
            if(checkString(v)){
                std::cout << "Accepted\n";
            }
            else{
                std::cout << "Not Accepted\n";
            }
        }
    }
    /* operator functions */
    friend std::istream& operator >>(std::istream& is, Parser<T>& parser){
        is >> *parser.gram_;
        parser.objectCreation();
        parser.success_ = (bool)*parser.gram_;
        return is;
    }
};


#endif //COMPILER_ALGORITHMS_PARSER_H
