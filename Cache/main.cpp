#include <iostream>
using namespace std;
#include <vector>
#include "cache.cpp"

void start_vector(vector<int> &vec) {
    for(size_t i = 0; i < 5; i++) {
        vec.push_back(2);
    }
}

int main() {
    vector<int> arr = {8, 7 ,12, 13, 9};
    
        // Element to move to index 0
    int i = 3;

    // Move element i to index 0
    int temp = arr[i];
    for (int j = i; j > 0; j--) {
        arr[j] = arr[j - 1];
    }
    arr[0] = temp;

    // Print array
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }

}