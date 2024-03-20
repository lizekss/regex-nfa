#include <string>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <iostream>

using namespace std;

class transition {
    public:
    char input;
    int from;
    int go_to;
    bool operator<(const transition &other) const{
        if (from < other.from) 
            return true;
        
        if (from == other.from && (go_to < other.go_to || go_to == other.go_to && input < other.input))
            return true;
        
        return false;
    }
    transition(int from, char input, int go_to) {
        this->from = from;
        this->input = input;
        this->go_to = go_to;
    }
};

class NFA {
    public:
        vector<set<transition> > states;
        vector<bool> final_states;
        int start_state;
        int final_state;

        NFA() {
            states = vector<set<transition> >();
            final_states = vector<bool>();
            start_state = 0;
            final_state = 0;
        }

        void set_start(int s) {
            if (s >= states.size())
                add_state(s);
            start_state = s;
        }

        void set_final(int f) {
            if (f >= states.size())
                add_state(f);
            final_states.resize(states.size());
            final_states[f] = true;
            final_state = f;
        }

        void add_state(int s) {
            if (s >= states.size())
                states.resize(s + 1);
            states[s] = set<transition>();
        }

        void add_transition(int from, char input, int go_to) {
            if (from >= states.size())
                add_state(from);
            if (go_to >= states.size())
                add_state(go_to);
            states[from].insert(transition(from, input, go_to));
        }

};

string simulate_nfa(NFA &nfa, string &s) {
    string result = "";
    for (int i = 0; i < s.length(); i++)
        result += "N";

    queue<pair<int, int> > q = queue<pair<int, int> >();
    q.push(make_pair(0, 0));

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        if (cur.second >= s.length())
            break;
        for (set<transition>::iterator it = nfa.states[cur.first].begin(); it != nfa.states[cur.first].end(); it++) {
            if (it->input == s[cur.second]) {
                q.push(make_pair(it->go_to, cur.second + 1));
                if (nfa.final_states[it->go_to])
                    result[cur.second] = 'Y';
            }
        }
    }
    return result;
}

int main() {
    string input;
    cin >> input;

    int n, a, t;
    cin >> n >> a >> t;
    NFA nfa = NFA();
    for (int i = 0; i < a; i++) {
        int f;
        cin >> f;
        nfa.set_final(f);
    }
    for (int i = 0; i < n; i++) {
        int m;
        cin >> m;
        for (int j = 0; j < m; j++) {
            char c;
            int to;
            cin >> c >> to;
            nfa.add_transition(i, c, to);
        }
    }

    cout << simulate_nfa(nfa, input);
    return 0;
}