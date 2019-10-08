#ifndef COMPILER_ALGORITHMS_PRINT_H
#define COMPILER_ALGORITHMS_PRINT_H

#include <ostream>
#include <vector>
#include <set>

namespace prettyprint{
    std::string line_start;
    std::string line_end;
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
    // automatas print functions
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::map<T*, std::map<std::string, std::set<T*>>> & delta){
        make_default();
        for(auto i:delta){
            for(auto j:i.second){
                os << "(" << *i.first << ", " << j.first << ") --> " << j.second << "\b\n";
            }
        }
        return os;
    }
    template <class T>
    inline std::ostream& operator <<(std::ostream& os, const std::map<T*, std::map<std::string, T*>> & delta){
        make_default();
        for(auto i:delta){
            for(auto j:i.second){
                os << "(" << *i.first << ", " << j.first << ") --> " << *j.second << "\n";
            }
        }
        return os;
    }
    // cfg print functions
    inline std::ostream& operator <<(std::ostream& os, const std::map<std::string, std::set<std::string>>& m){
        make_default();
        for(const auto & i : m){
            os << i.first << " --> "<< i.second << "\n";
        }
        return os;
    }
}
#endif //COMPILER_ALGORITHMS_PRINT_H
