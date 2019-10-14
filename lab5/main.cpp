#include <iostream>
#include "matrix.hpp"

using std::cout;
using std::endl;

int main() {
    Matrix A(1000);
//    A.print();
//    cout << endl;

//    A.StrassenMultiply(A);
//    A.print();

    Matrix B(1000);
//    B.print();
//    cout << endl;

//    Matrix C = A * B;
//    C.print();
//    cout << endl;

    Matrix C = A.StrassenMultiply(B);
//    C.print();
    return 0;
}