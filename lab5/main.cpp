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
    float t;
    using namespace std::chrono;
    duration<double> time_span;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    Matrix C = A.StrassenMultiply(B);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    t = time_span.count();
    cout << "It tooks me" <<t << "seconds";
//    C.print();
    return 0;
}