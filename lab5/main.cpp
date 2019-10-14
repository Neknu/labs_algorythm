#include <iostream>
#include "matrix.hpp"

using std::cout;
using std::endl;

int main() {
    Matrix A(4);
    A.print();
    cout << endl;

//    A.StrassenMultiply(A);
//    A.print();

    Matrix B(4);
    B.print();
    cout << endl;

    Matrix C = A * B;
    C.print();
    cout << endl;

    C = A.StrassenMultiply(B);
    C.print();
    return 0;
}