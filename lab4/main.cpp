#include <iostream>
#include <functional>
#include <chrono>
#include <random>
#include <algorithm>

int rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(0, max);
    return dist(gen);
}


using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Field{
public:
    int key;
    int value;
    Field() {
        key = 0;
        value = 0;
    }

    Field(int k, int v) {
        key = k;
        value = v;
    }

};

class Array{
private:
    vector<Field> arr;
    int size;

public:
    Array() {
        size = 0;
    }

    Array(int N){
        for(int i = 0; i < N; i++) {
            arr.push_back(Field(rand_num(2), i));
        }
        size = N;
    }

    void TwoArraySort() {
        Array* zero = new Array();
        Array* one = new Array();

        for(int i = 0; i < size; i++) {
            if(arr[i].key == 0) {
                zero->arr.push_back(arr[i]);
                zero->size++;
            }
            else
            if(arr[i].key == 1) {
                one->arr.push_back(arr[i]);
                one->size++;
            }
        }
        arr.clear();
        for(int i = 0; i < zero->size; i++)
            arr[i] = zero->arr[i];

        for(int i = 0; i < one->size; i++)
            arr[zero->size + i] = one->arr[i];
    }

    void TwoSideSort() {
        int low = 0;
        int high = size - 1;

        while(low < high) {
            if (arr[low].key == 0) {
                low++;
            }
            else
            if (arr[high].key == 1) {
                high--;
            }
            else {
                std::swap(arr[low], arr[high]);
            }
        }
    }

    void BubbleSort() {
        for(int i = 0; i < size; i++)
            for(int j = i+1; j < size; j++)
                if(arr[j-1].key > arr[j].key)
                    std::swap(arr[j-1], arr[j]);
    }

    void print() {
        cout << endl << "print Array:" << endl;
        for(int i = 0; i < size; i++) {
            cout << arr[i].key << " " << arr[i].value << endl;
        }
    }
};

int main() {

    Array* A = new Array(100);
    A->print();
    A->TwoArraySort();
    A->print();
//
//    Array* A = new Array(100);
//    A->print();
//    A->TwoSideSort();
//    A->print();
//
//    Array* A = new Array(100);
//    A->print();
//    A->BubbleSort();
//    A->print();
    return 0;
}