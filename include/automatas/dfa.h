//
// Created by hiren on 09/01/20.
//

#ifndef COMPILER_ALGORITHMS_DFA_H
#define COMPILER_ALGORITHMS_DFA_H

#include <string>
#include <set>
#include <map>
#include <ostream>

template <class STATE, class ALPHABET=std::string>
class DFA {
private:
    /* DFA Properties (Σ, Q, δ, q0, F) */
    std::set<ALPHABET> sigma_;
    std::set<STATE> states_;
    STATE starting_state_;
    std::set<STATE> final_states_;
    std::map<STATE, std::map<ALPHABET, STATE>> delta_;

public:
    /* constructors for DFA class */

    explicit DFA(){
        sigma_.clear();
        states_.clear();
        final_states_.clear();
        delta_.clear();
    }

    /* type conversion */

    explicit operator bool(){
        return not sigma_.empty() and not states_.empty() and not final_states_.empty() and not delta_.empty();
    }

    /* setter and getter functions */

    void setSigma(const std::set<ALPHABET> &sigma) {
        sigma_ = sigma;
    }

    void setStates(const std::set<STATE> &states) {
        states_ = states;
    }

    void setStartingState(STATE startingState) {
        starting_state_ = startingState;
    }

    void setFinalStates(const std::set<STATE> &finalStates) {
        final_states_ = finalStates;
    }

    void setDelta(const std::map<STATE, std::map<ALPHABET, STATE>> &delta) {
        delta_ = delta;
    }

    const std::set<ALPHABET> &getSigma() const {
        return sigma_;
    }

    const std::set<STATE> &getStates() const {
        return states_;
    }

    STATE getStartingState() const {
        return starting_state_;
    }

    const std::set<STATE> &getFinalStates() const {
        return final_states_;
    }

    const std::map<STATE, std::map<ALPHABET, STATE>> &getDelta() const {
        return delta_;
    }

    /* adder functions */

    void addAlphabet(const ALPHABET &sigma) {
        sigma_.insert(sigma);
    }

    void addState(const STATE &states) {
        states_.insert(states);
    }

    void addFinalState(const STATE &finalStates) {
        final_states_.insert(finalStates);
    }

    void addTransition(const STATE &state1,const ALPHABET &alphabet,const STATE &state2) {
        delta_[state1][alphabet] = state2;
    }

    /* info getter functions */
    const STATE &getTransition(const STATE &state,const ALPHABET &alphabet) const {
        return delta_[state][alphabet];
    }

};


#endif //COMPILER_ALGORITHMS_DFA_H
