#include <iostream>
using namespace std;
#include <vector>

void start_vector(vector<int> &vec) {
    for(size_t i = 0; i < 5; i++) {
        vec.push_back(2);
    }
}

int main() {
    vector<int> x = {0, 1, 2, 3};
    
    start_vector(x);

    for(size_t i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }

}