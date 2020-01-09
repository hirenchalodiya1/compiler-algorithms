#include <ostream>
#include <vector>
#include <set>
#include <map>
#include <prettyprint/print.h>


void prettyprint::make_default(){
    prettyprint::line_start = "";
    prettyprint::line_end = " ";
}

template <class T>
std::ostream& prettyprint::operator<<(std::ostream& os, const std::vector<T>& s){
    for(const auto& i:s){
        os << prettyprint::line_start << i << prettyprint::line_end;
    }
    return os;
}

template <class T>
inline std::ostream& prettyprint::operator<<(std::ostream& os, const std::vector<T*>& s){
    for(auto *i:s){
        os << prettyprint::line_start << *i << prettyprint::line_end;
    }
    return os;
}
