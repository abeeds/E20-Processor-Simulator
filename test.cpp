#include <iostream>
using namespace std; 



int main() {
    // 0010010011111111
    uint16_t an_instruc = 9461;

    an_instruc = an_instruc << 12;
    an_instruc = an_instruc >> 12;

    cout << an_instruc << endl;

}