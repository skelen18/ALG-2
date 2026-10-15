#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using std::vector, std::array;

class State {
    public:
    array<bool, 4> state;

    bool isValid() const {
        if ((state[1] == state[2]) and (state[0] != state[1])) {
            return false;
        }
        if ((state[1] == state[3]) and (state[0] != state[1])) {
            return false;
        }
        return true;
    }

    bool isFinal() const {
        for(size_t i = 0; i < state.size(); ++i){
            if(not state[i]){
                return false;
            }
        }
        return true;
    }

    State() {
        for(size_t i = 0; i < state.size(); ++i){
            state[i] = false;
        }
    }

    State(const int stateInt){
        int power = 1;
        for(size_t i = 0; i < state.size(); ++i){
            state[i] = (stateInt & power) != 0;
            power *= 2;
        }
    }

    vector<State> nextStates() const {
        vector<State> nextStates;
        State newState;

        newState.state = state;
        newState.state[0] = not state[0];
        if( newState.isValid() ){
            nextStates.push_back(newState);
        }

        for(size_t i = 1; i < state.size(); ++i){
            if( state[0] != state[i] ) continue;
            newState.state = state;
            newState.state[0] = not state[0];
            newState.state[i] = not state[i];
            if( newState.isValid() ){
                nextStates.push_back(newState);
            }
        }
        return nextStates;
    }

    int toInt() const {
        int res = 0;
        int power = 1;
        for(size_t i = 0; i < state.size(); i++){
            if(state[i]){
                res += power;
            }
            power *= 2;
        }

        return res;
    }

    void print() const {
        std::unordered_map<int, std::string> itemNames = {
            {0, "P"},
            {1, "k"},
            {2, "v"},
            {3, "z"}
        };
        std::string leftSide = "";
        std::string rightSide = "";
        for(size_t i = 0; i < state.size(); ++i){
            if(state[i]){
                leftSide += itemNames.at(i) + " ";
            } else {
                rightSide += itemNames.at(i) + " ";
            }
        }
        std::cout << "Left side: " << leftSide << " ";
        std::cout << "Right side: " << rightSide << "\n";
    }
};

vector<State> solveKVZ(){
    vector<State> path;
    std::queue<State> bfsQ;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parents;

    State init;
    bfsQ.push(init);
    visited.insert(init.toInt());
    parents[init.toInt()] = init.toInt();

    while( not bfsQ.empty() ){
        State current = bfsQ.front();
        bfsQ.pop();

        if( current.isFinal() ){
            path.push_back(current);

            int stateInt = current.toInt();
            int parentInt = parents[stateInt];

            while( parentInt != stateInt ){
                State localState(parentInt);

                path.push_back(localState);
                stateInt = parentInt;
                parentInt = parents[stateInt];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        for(const State& next : current.nextStates()){
            int nextStateInt = next.toInt();
            if( visited.count(nextStateInt) == 0 ){
                visited.insert(nextStateInt);
                parents[nextStateInt] = current.toInt();
                bfsQ.push(next);
            }
        }
    }

    return path;
}

int main(){
    vector<State> solution_path = solveKVZ();

    for(const State& state : solution_path){
        state.print();
    }
    return 0;
}