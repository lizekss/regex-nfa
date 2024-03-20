#include <string>
#include <stack>
#include <map>
#include <deque>
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

int precedence(char op) { 
    switch (op) {
        case '*': return 3;
        case '.': return 2;
        case '|': return 1;
        default: return -1;
    }
} 

// epsilon - '$', concat - '.'
string infix_to_postfix(string inf) { 
  stack<char> s;
  string result; 
  for(int i = 0; i < inf.length(); i++) { 
    char c = inf[i];
    if(c == '*' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9') 
        result += c; 
        
    // detect epsilon
    else if(c == '(' && i < inf.length() - 1 && inf[i + 1] == ')')
        result += '$';

    else if(c == '(') 
        s.push(c); 
  
    else if(c == ')') { 
      while(!s.empty() && s.top() != '(') { 
        result += s.top(); 
        s.pop(); 
      }
      if (!s.empty())
        s.pop();
    } 
     
    else { 
      while (!s.empty() && precedence(inf[i]) <= precedence(s.top())) { 
        result += s.top();
        s.pop(); 
      } 
      s.push(c); 
    } 
  } 
  
  while(!s.empty()) 
  { 
    result += s.top();
    s.pop(); 
  } 
  
    return result;
}

string preprocess(string re) {
    string result = "";
    for (int i = 0; i < re.size() - 1; i++) {
        char left = re[i];
        char right = re[i + 1];
        result += left;
        bool leftIsOperand = (left >= 'a' && left <= 'z' || left >= '0' && left <= '9');
        
        if (leftIsOperand || left == ')' || left == '*') {
            bool rightIsOperand = (right >= 'a' && right <= 'z' || right >= '0' && right <= '9');
            if (rightIsOperand || right == '(') {
                result += '.';
            }
        }
    }
    result += re[re.size() - 1];
    return result;
}

NFA unit_nfa(char c) {
  NFA result = NFA();
  result.set_start(0);
  result.set_final(1);
  result.add_transition(0, c, 1);
  return result;
}

void merge_transitions(NFA &result, NFA &other, int offset) {
  for (vector<set<transition> >::iterator it =other.states.begin(); it != other.states.end(); ++it)
    for (set<transition>::iterator it2 = it->begin(); it2 != it->end(); it2++)
        result.add_transition(it2->from + offset, it2->input, it2->go_to + offset);
}

NFA do_kleene(NFA &a) {
  NFA result = NFA();
  result.set_start(0);
  result.set_final(a.states.size() + 1);
  merge_transitions(result, a, 1);
  result.add_transition(result.start_state, '$', a.start_state + 1); // +
  result.add_transition(a.final_state + 1, '$', result.final_state); // +
  result.add_transition(a.final_state + 1, '$', a.start_state + 1);
  result.add_transition(result.start_state, '$', result.final_state);
  return result;
}

NFA do_concat(NFA &a, NFA &b) {
  NFA result = NFA();
  int offset = a.states.size();
  result.set_start(a.start_state);
  result.set_final(b.final_state + offset);
  merge_transitions(result, a, 0);
  merge_transitions(result, b, offset);
  result.add_transition(a.final_state, '$', b.start_state + offset);
  return result;
}

NFA do_union(NFA &a, NFA &b) {
  NFA result = NFA();
  int offset = a.states.size() + 1;
  result.set_start(0);
  result.set_final(max(a.states.size(), b.states.size() + offset));
  merge_transitions(result, a, 1);
  merge_transitions(result, b, offset);
  result.add_transition(0, '$', a.start_state + 1);
  result.add_transition(0, '$', b.start_state + offset);
  result.add_transition(a.final_state + 1, '$', result.final_state);
  result.add_transition(b.final_state + offset, '$', result.final_state);
  return result;
}

void get_epsilon_closure(int cur, NFA &a, vector<bool> &visited, set<int> &result) {
  if (visited[cur]) return;
  
  visited[cur] = true;

  for(set<transition>::iterator it = a.states[cur].begin(); it != a.states[cur].end(); it++) {
      if (it->input == '$') {
        int go_to = it->go_to;
        result.insert(go_to);
        get_epsilon_closure(go_to, a, visited, result);
      }
  }
}

void remove_null_transitions(int s, NFA &a) {
    vector<bool> visited = vector<bool>(a.states.size());
    set<int> epsilon_closure = set<int>();
    get_epsilon_closure(s, a, visited, epsilon_closure);

    for(set<transition>::iterator it = a.states[s].begin(); it != a.states[s].end();) {
      if (it->input == '$') {
        a.states[s].erase(it++);
      } else ++it;
    }

    for(set<int>::iterator it = epsilon_closure.begin(); it != epsilon_closure.end(); it++) {
        if (a.final_states[*it])
          a.final_states[s] = true;
        
        for (set<transition>::iterator it2 = a.states[*it].begin(); it2 != a.states[*it].end(); it2++)
          if (it2->input != '$')
              a.states[s].insert(transition(s, it2->input, it2->go_to));
    }
}

void reduce(NFA &a) {
    a.final_states = vector<bool>(a.states.size(), false);
    a.final_states[a.final_state] = true;
    for (int i = 0; i < a.states.size(); i++)
        remove_null_transitions(i, a);
    
    // remove unreachable states
    vector<bool> unreachable = vector<bool>(a.states.size(), true);
    unreachable[0] = false;

    for (int i = 0; i < a.states.size(); i++)
      for (set<transition>::iterator it = a.states[i].begin(); it != a.states[i].end(); it++)
        unreachable[it->go_to] = false;
    
    for (int i = unreachable.size() - 1; i > 0; i--) {
      if (unreachable[i]) {
        a.states.erase(a.states.begin() + i);
        a.final_states.erase(a.final_states.begin() + i);
        for (int j = 0; j < a.states.size(); j++) {
          set<transition> updated = set<transition>();
          for (set<transition>::iterator it = a.states[j].begin(); it != a.states[j].end(); it++) {
              transition t = *it;
              if (it->from > i) t.from = it->from - 1;
              if (it->go_to > i) t.go_to = it->go_to - 1;
              updated.insert(t);
          }
          a.states[j] = updated;
        }
      }
    }
}

NFA regex_to_nfa(string re) {
  re = preprocess(re);
  re = infix_to_postfix(re);
  stack<NFA> s = stack<NFA>();
    for (int i = 0; i < re.size(); i++) {
      char c = re[i];
      if (c == ' ' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '$') {
        s.push(unit_nfa(c));
      } else if (c == '*' && !s.empty()) {
        NFA top = s.top();
        s.pop();
        s.push(do_kleene(top));
      } else if (c == '|' && !s.empty()) {
        NFA b = s.top();
        s.pop();
        if (!s.empty()) {
          NFA a = s.top();
          s.pop();
          s.push(do_union(a, b));
        }
      } else if (c == '.' && !s.empty()) {
        NFA b = s.top();
        s.pop();
        if (!s.empty()) {
          NFA a = s.top();
          s.pop();
          s.push(do_concat(a, b));
        }
      }
    }
  if (!s.empty()) {
    reduce(s.top());
    return s.top();
  }
    
  return NFA();
}

void print_nfa(NFA &a) {
  int num_final = 0;
  for (int i = 0; i < a.final_states.size(); i++)
    if (a.final_states[i]) num_final++;

  int num_trans = 0;
  for (int i = 0; i < a.states.size(); i++)
    num_trans += a.states[i].size();

  cout << a.states.size() << " " << num_final << " " << num_trans << endl;
  for (int i = 0; i < a.final_states.size(); i++)
    if (a.final_states[i]) cout << i << " ";

  cout << endl;
  for (vector<set<transition> >::iterator it =a.states.begin(); it != a.states.end(); ++it) {
      cout << it->size();
      for (set<transition>::iterator it2 = it->begin(); it2 != it->end(); it2++)
        cout << " " << it2->input << " " << it2->go_to;
      cout << endl;
  }
}

int main() 
{ 
  string re;
  cin >> re;
  NFA a = regex_to_nfa(re);
  print_nfa(a);
  return 0; 
} 