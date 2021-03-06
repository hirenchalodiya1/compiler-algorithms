#ifndef COMPILER_ALGORITHMS_PRINT_CPP
#define COMPILER_ALGORITHMS_PRINT_CPP

#include <ostream>
#include <vector>
#include <set>
#include <map>
#include <automatas/dfa.h>
#include <automatas/nfa.h>

namespace prettyprint{
    std::string line_start;
    std::string line_end;
    void make_default(){
        line_start = "";
        line_end = " ";
    }

    // vector print
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
    // set print
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
    // automatas print functions --> delta
    template <class STATE, class ALPHABET>
    inline std::ostream& operator <<(std::ostream& os, const std::map<STATE, std::map<ALPHABET, std::set<STATE>>> & delta){
        make_default();
        for(auto i:delta){
            for(auto j:i.second){
                os << "(" << i.first << ", " << j.first << ") --> " << j.second << "\b\n";
            }
        }
        return os;
    }
    template <class STATE, class ALPHABET>
    inline std::ostream& operator <<(std::ostream& os, const std::map<STATE, std::map<ALPHABET, STATE>> & delta){
        make_default();
        for(auto i:delta){
            for(auto j:i.second){
                os << "(" << i.first << ", " << j.first << ") --> " << j.second << "\n";
            }
        }
        return os;
    }

    template<class STATE, class ALPHABET>
    std::ostream&operator<<(std::ostream& os, DFA<STATE, ALPHABET>& dfa){
        make_default();
        os << "------------------------------DFA------------------------------\n"
           << "Symbols:\n\t" << dfa.getSigma() << "\b\n"
           << "Starting State: " << dfa.getStartingState() << "\n"
           << "States:\n\t" << dfa.getStates() << "\b\n"
           << "Final states:\n\t" << dfa.getFinalStates() << "\b\n"
           << "Delta Transitions:\n" << dfa.getDelta()
           << "---------------------------------------------------------------\n";
        return os;
    }

    template<class STATE, class ALPHABET>
    std::ostream&operator<<(std::ostream &os, NFA<STATE, ALPHABET> &nfa){
        make_default();
        os << "------------------------------NFA------------------------------\n"
           << "Symbols:\n\t" << nfa.getSigma() << "\b\n"
           << "Starting State: " << nfa.getStartingState() << "\n"
           << "States:\n\t" << nfa.getStates() << "\b\n"
           << "Final states:\n\t" << nfa.getFinalStates() << "\b\n"
           << "Delta Transitions:\n" << nfa.getDelta()
           << "---------------------------------------------------------------\n";
        return os;
    }

    // cfg print functions
    inline std::ostream& operator <<(std::ostream& os, const std::map<std::string, std::set<std::string>>& m) {
        make_default();
        for (const auto &i : m) {
            os << i.first << " --> " << i.second << "\n";
        }
        return os;
    }
}

#endif //COMPILER_ALGORITHMS_PRINT_CPP
