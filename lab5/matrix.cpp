//
// Created by Asus on 14.10.2019.
//

#include "matrix.hpp"

int rand_num(int max) {
static std::random_device rd;
static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
static std::mt19937_64 gen(seed);
std::uniform_real_distribution<double> dist(0, max);
return dist(gen);
}

int find_size(int size) {
    int res = 1;
    while(res < size)
        res *= 2;
    return res;
}

void Matrix::print() {
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            std::cout << arr[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Matrix Matrix::operator * (Matrix& another) {
    Matrix res(size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            int sum = 0;
            for(int k = 0; k < size; k++) {
                sum += arr[i][k] * another.arr[k][j];
            }
            res.arr[i][j] = sum;
        }
    }
    return res;
}

Matrix Matrix::operator + (Matrix& another) {
    Matrix res(size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            res.arr[i][j] = arr[i][j] + another.arr[i][j];
        }
    }
    return res;
}

Matrix Matrix::operator - (Matrix& another) {
    Matrix res(size);
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            res.arr[i][j] = arr[i][j] - another.arr[i][j];
        }
    }
    return res;
}

Matrix Matrix::StrassenMultiply(Matrix another) {
    int old_size = size;
    int new_size = find_size(size);
    arr.resize(new_size);
    size = new_size;
    another.arr.resize(new_size);
    another.size = new_size;
    for (int i = 0; i < new_size; ++i) {
        arr[i].resize(new_size);
        another.arr[i].resize(new_size);
    }

    Matrix res = StrassenMultiplyRekurs(*this, another);
    arr.resize(old_size);
    size = old_size;
    another.arr.resize(old_size);
    another.size = old_size;
    res.size = old_size;
    res.arr.resize(old_size);

    for (int i = 0; i < new_size; ++i) {
        arr[i].resize(old_size);
        another.arr[i].resize(old_size);
        res.arr[i].resize(old_size);
    }
    return res;
}

Matrix Matrix::StrassenMultiplyRekurs(Matrix A, Matrix B) {
    if(A.size <= SIZE)
        return A*B;
    Matrix A11(A, A.size / 2, 0, 0);
    Matrix A12(A, A.size / 2, 0, A.size / 2);
    Matrix A21(A, A.size / 2, A.size / 2, 0);
    Matrix A22(A, A.size / 2, A.size / 2, A.size / 2);


    Matrix B11(B, B.size / 2, 0, 0);
    Matrix B12(B, B.size / 2, 0, B.size / 2);
    Matrix B21(B, B.size / 2, B.size / 2, 0);
    Matrix B22(B, B.size / 2, B.size / 2, B.size / 2);


    Matrix P1 = StrassenMultiplyRekurs(A11 + A22, B11 + B22);
    Matrix P2 = StrassenMultiplyRekurs(A21 + A22, B11);
    Matrix P3 = StrassenMultiplyRekurs(A11, B12 - B22);
    Matrix P4 = StrassenMultiplyRekurs(A22, B21 - B11);
    Matrix P5 = StrassenMultiplyRekurs(A11 + A12, B22);
    Matrix P6 = StrassenMultiplyRekurs(A21 - A11, B11 + B12);
    Matrix P7 = StrassenMultiplyRekurs(A12 - A22, B21 + B22);

    Matrix C11 = P1 + P4 - P5 + P7;
    Matrix C12 = P3 + P5;
    Matrix C21 = P2 + P4;
    Matrix C22 = P1 - P2 + P3 + P6;


    return Matrix(C11, C12, C21, C22);
}
