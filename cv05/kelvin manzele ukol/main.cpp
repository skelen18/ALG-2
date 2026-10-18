#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using std::vector;

class State {
    public:
    int n;
    vector<bool> state;

    bool isValid() const {
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if (state[1 + n + i] == state[1 + j]) {
                    if (state[1 + n + i] != state[1 + i] and state[1 + j] != state[1 + n + j]){
                        return false;
                    }
                }
            }
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

    State(int pocet = 2) {
        n = pocet;
        state.resize(2 * n + 1, false);
    }

    State(const int stateInt, int pocet){
        n = pocet;
        state.resize(2 * n + 1, false);
        int power = 1;
        for(size_t i = 0; i < state.size(); ++i){
            state[i] = (stateInt & power) != 0;
            power *= 2;
        }

    }

    vector<State> nextStates() const {
        vector<State> nextStates;
        State newState(n);

        for(size_t i = 1; i < state.size(); ++i){
            if( state[0] != state[i] ) continue;
            newState.state = state;
            newState.state[0] = not state[0];
            newState.state[i] = not state[i];
            if( newState.isValid() ){
                nextStates.push_back(newState);
            }
        }

        for(int i = 0; i < n; ++i){
            if( state[0] == state[1 + i] and state[0] == state[1 + n + i] ){
                newState.state = state;
                newState.state[0] = not state[0];
                newState.state[1 + i] = not state[1 + i];
                newState.state[1 + n + i] = not state[1 + n + i];
                if( newState.isValid() ){
                    nextStates.push_back(newState);
                }
            }
        }


        for(int i = 0; i < n; ++i){
            for(int j = i + 1; j < n; ++j){
                if( state[0] == state[1 + i] and state[0] == state[1 + j] ){
                    newState.state = state;
                    newState.state[0] = not state[0];
                    newState.state[1 + i] = not state[1 + i];
                    newState.state[1 + j] = not state[1 + j];
                    if( newState.isValid() ){
                        nextStates.push_back(newState);
                    }
                }

                if( state[0] == state[1 + n + i] and state[0] == state[1 + n + j] ){
                    newState.state = state;
                    newState.state[0] = not state[0];
                    newState.state[1 + n + i] = not state[1 + n + i];
                    newState.state[1 + n + j] = not state[1 + n + j];
                    if( newState.isValid() ){
                        nextStates.push_back(newState);
                    }
                }
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

};

vector<State> solveKVZ(int n){
    vector<State> path;
    std::queue<State> bfsQ;
    std::unordered_set<int> visited;
    std::unordered_map<int, int> parents;

    State init(n);
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
                State localState(parentInt, n);

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

    for(int n = 2; n <= 4; n++){
        vector<State> solution_path = solveKVZ(n);
        int pocet = solution_path.size() - 1;
        std::cout << n << " pairs - " << pocet << " transports\n";
    }
    std::cout << "\n";

    return 0;
}