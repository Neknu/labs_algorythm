
#ifndef LAB5_MATRIX_HPP
#define LAB5_MATRIX_HPP

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>

int const RAND = 10;
int const SIZE = 4;
using std::vector;

int rand_num(int max) ;

class Matrix {
private:
    int size;
    vector< vector<int> > arr;
    Matrix StrassenMultiplyRekurs(Matrix A, Matrix B) ;
public:
    Matrix(int s) {
        size = s;
        vector<vector<int>> matrix(size, vector<int>(size));
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                matrix[i][j] = rand_num(RAND);
        arr = matrix;
    }

    Matrix(Matrix A, int s, int top, int left) {
        size = s;
        vector<vector<int>> matrix(size, vector<int>(size));
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                matrix[i][j] = A.arr[top + i][left + j];
        arr = matrix;
    }

    Matrix(Matrix C11, Matrix C12, Matrix C21, Matrix C22) {
        size = C11.size * 2;
        vector<vector<int>> matrix(size, vector<int>(size));
        for(int i = 0; i < size / 2; i++)
            for(int j = 0; j < size / 2; j++) {
                matrix[i][j] = C11.arr[i][j];
                matrix[i][j + size / 2] = C12.arr[i][j];
                matrix[i + size / 2][j] = C21.arr[i][j];
                matrix[i + size / 2][j + size / 2] = C22.arr[i][j];
            }
        arr = matrix;
    }

    ~Matrix() {
        arr.clear();
    }


    void print() ;
    Matrix operator * (Matrix& another);
    Matrix operator + (Matrix& another);
    Matrix operator - (Matrix& another);
    Matrix StrassenMultiply(Matrix another) ;

};

#endif //LAB5_MATRIX_HPP
