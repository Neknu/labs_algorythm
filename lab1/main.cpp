#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <random>
#include <algorithm>
#include <functional>
#include <chrono>

using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::ios;

int const COUNT = 10000;
int const SIZE = 1000;
int const FILES = 7; // less than 10 plz

int fib[1000000][FILES] = {0};
int arr[SIZE] = {0};

int rand_num(int max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(0, max);
    return dist(gen);
}

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}


void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void create_input() {
    const char* FILENAM = "example.bin";
    ofstream output(FILENAM,ios::binary);

    for(int i = 0; i < COUNT; i++) {
        int toStore = rand_num(INT_MAX);
        output.write((char *) &toStore, sizeof(toStore));
    }

    output.close();
}

int sum_fib(int i) {
    int res = 0;
    for(int j = 0; j < FILES; j++) {
        res += fib[i][j];
    }
    return res;
}

int find_max(int i) {
    int res = 0;
    for(int j = 0; j < FILES; j++) {
        if(fib[i][j] > fib[i][res])
            res = j;
    }
    return res;
}

void share_data_fibonachi() {
    int rec = (COUNT / SIZE) + 1;
    fib[0][0] = 1;
    int pivot = -1;
    int i = 0;
    while(sum_fib(i) <= rec) {
        pivot = find_max(i);
        for(int j = 0; j < FILES; j++) {
            if(j == pivot) {
                fib[i+1][j] = 0;
            }
            else {
                fib[i+1][j] = fib[i][j] + fib[i][pivot];
            }
        }
        i++;
    }

    for(int j = 0; j <= i; j++){
        for(int k = 0; k < FILES; k++){
            cout << fib[j][k] << " ";
        }
        cout << endl;
    }

    int FIELDS = sum_fib(i);
    int RECORDS = (COUNT / FIELDS) + 1;
    int position = i;
    cout << "count of fields:" << FIELDS << endl;
    cout << "count of elem in one field:" << RECORDS << endl;


    string filename;

    ofstream write_files[FILES];
    for(int i = 0; i < FILES; i++){
        string filename;

        filename = "data" + std::to_string(i) + ".bin";
        write_files[i].open(filename, ios::binary);
        write_files[i].close();
    }

    ifstream read_files[FILES];
    int toRestore = 0;
    for(int i = 0; i < FILES; i++){
        filename = "data" + std::to_string(i) + ".bin";
        read_files[i].open(filename, ios::binary);
        read_files[i].close();
    }



    const char* FILENAM = "example.bin";

    toRestore=0;
    int count_elem = 0;
    ifstream input(FILENAM, ios::binary);
    for(int l = 0; l < FILES; l++) {
        filename = "data" + std::to_string(l) + ".bin";
        write_files[l].open(filename, ios::binary);
        for (int i = 0; i < fib[position][l]; i++) {
            int j;
            for (j = 0; j < RECORDS; j++) {
                if (count_elem == COUNT) {
                    break;
                }
                input.read((char *) &toRestore, sizeof(toRestore));
                count_elem++;
                arr[j] = toRestore;
            }
            quickSort(arr, 0, j - 1);

            for (int i = 0; i < j - 1; i++) {
                cout << arr[i] << " ";
                write_files[l].write((char *) &arr[i], sizeof(arr[i]));
            }
            cout << endl;
            int br = -1;
            write_files[l].write((char *) &br, sizeof(br));
        }
        int br = -1;
        write_files[l].write((char *) &br, sizeof(br));
        write_files[l].close();
    }
}



int main (){

    share_data_fibonachi();


    return 0;
}