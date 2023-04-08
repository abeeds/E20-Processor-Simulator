#include <iostream>
#include <vector>
#include <stack>
using namespace std; 

bool isValid(string s) {
    vector<char> brackets;
    stack<char> order;

    for(size_t i = 0; i < s.size(); i++) {
        if(s[i] == '(' || s[i] == ')' || s[i] == '[' || s[i] == ']'|| s[i] == '{' || s[i] == '}') {
            brackets.push_back(s[i]);
        }
    }
    

    for(size_t i = 0; i < brackets.size(); i++) {
        // if opening push bracket 
        if(brackets[i] == '(' || brackets[i] == '[' || brackets[i] == '{') {
            order.push(brackets[i]);
            continue;
        }


        if(brackets[i] == ')' && order.top() == '('){
            order.pop();
            continue;
        }

        if(brackets[i] == ']' && order.top() == '[') {
            order.pop();
            continue;
        }

        if(brackets[i] == '}' && order.top() == '{') {
            order.pop();
            continue;
        }
    }
    return order.size() == 0;
    
}


int main() {
    string test = "([]";
    if(isValid(test)) {
        cout << "True" << endl;
    }
    else{
        cout << "False" << endl;
    }
}