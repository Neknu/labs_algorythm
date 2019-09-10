#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>

using std::ofstream;
using std::ifstream;
using std::cin;
using std::cout;
using std::string;

int const SIZE = 1000;
int const STEPS = 64;
int arr[SIZE + 2];

int rand_num(int max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(0, max);
    return dist(gen);
}

int main() {
    string path = "cmake-build-debug/input.txt";
    ifstream input;
    ofstream output;
    ifstream A_read;
    ifstream B_read;
    ifstream C_read;
    ifstream D_read;
    ofstream A_write;
    ofstream B_write;
    ofstream C_write;
    ofstream D_write;

    output.open("input.txt");

    for(int i = 0; i < STEPS*SIZE; i++) {
        output << rand_num(INT_MAX) << " ";
    }

    input.open("input.txt");
    if(!input.is_open())
        cout << "error";

    string curr_read_fold = "A";
    string curr_write_fold = "A";

    while(!input.eof()) {
        for(int i = 0; i < SIZE; i++)
            input >> arr[i];

        std::sort(arr, arr + SIZE);

        if(curr_write_fold == "A") {
            A_write.open("A.txt", std::ofstream::app);
            for(int i = 0; i < SIZE; i++)
                A_write << arr[i] << " ";
            A_write.close();
            curr_write_fold = "B";
        }
        else {
            if (curr_write_fold == "B") {
                B_write.open("B.txt", std::ofstream::app);
                for (int i = 0; i < SIZE; i++)
                    B_write << arr[i] << " ";
                B_write.close();
                curr_write_fold = "A";
            }
        }
    }

    input.close();
    return 0;
}