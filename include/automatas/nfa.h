
#ifndef COMPILER_ALGORITHMS_NFA_H
#define COMPILER_ALGORITHMS_NFA_H

#include <string>
#include <set>
#include <map>
#include <stack>
#include <ostream>

template <class STATE, class ALPHABET=std::string>
class NFA{
private:
    /* NFA Properties (Σ, Q, δ, q0, F) */
    std::set<ALPHABET> sigma_;
    std::set<STATE> states_;
    STATE starting_state_;
    std::set<STATE> final_states_;
    std::map<STATE, std::map<ALPHABET, std::set<STATE>>> delta_;
    ALPHABET epsilon_;

public:
    /* constructor for NFA class */

    explicit NFA(){
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

    const std::set<ALPHABET> &getSigma() const {
        return sigma_;
    }

    void setSigma(const std::set<ALPHABET> &sigma) {
        sigma_ = sigma;
    }

    const std::set<STATE> &getStates() const {
        return states_;
    }

    void setStates(const std::set<STATE> &states) {
        states_ = states;
    }

    STATE getStartingState() const {
        return starting_state_;
    }

    void setStartingState(STATE startingState) {
        starting_state_ = startingState;
    }

    const std::set<STATE> &getFinalStates() const {
        return final_states_;
    }

    void setFinalStates(const std::set<STATE> &finalStates) {
        final_states_ = finalStates;
    }

    const std::map<STATE, std::map<ALPHABET, std::set<STATE>>> &getDelta() const {
        return delta_;
    }

    void setDelta(const std::map<STATE, std::map<ALPHABET, std::set<STATE>>> &delta) {
        delta_ = delta;
    }

    ALPHABET getEpsilon() const {
        return epsilon_;
    }

    void setEpsilon(ALPHABET epsilon) {
        epsilon_ = epsilon;
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
        delta_[state1][alphabet].insert(state2);
    }

    /* additional functions */

    bool isValidState(const STATE& state){
        return states_.find(state) != states_.end();
    }

    bool isValidAlphabet(const ALPHABET& alphabet){
        return sigma_.find(alphabet) != sigma_.end() or alphabet == epsilon_;
    }

    std::set<STATE> getEpsilonClosure(std::set<STATE>& state);

    std::set<STATE> &getTransition(const STATE &state,const ALPHABET &alphabet){
        return delta_[state][alphabet];
    }
};


template <class STATE, class ALPHABET>
std::set<STATE> NFA<STATE, ALPHABET>::getEpsilonClosure(std::set<STATE>& states)  {
    std::set<STATE> ans;
    ans.clear();
    std::stack<STATE> st;
    STATE temp;
    for(auto i:states){
        ans.insert(i);
        st.push(i);
    }
    while(!st.empty()){
        temp = st.top();
        st.pop();
        for(STATE i:delta_[temp][epsilon_]){
            if(ans.find(i) == ans.end()){
                ans.insert(i);
                st.push(i);
            }
        }
    }
    return ans;
}
#endif //COMPILER_ALGORITHMS_NFA_H
