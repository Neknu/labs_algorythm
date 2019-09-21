#include <iostream>

using namespace std;

void fill_arrays(int count, int arr_of_bolts[], int arr_of_nuts[]){
    cout << "Enter sizes of bolts:\n";
    for (int i = 0; i < count; i++){
        cout << "Bolt [" << i+1 << "]:";
        cin >> arr_of_bolts[i];
    }
    cout << "Enter sizes of nuts:\n";
    for (int i = 0; i < count; i++){
        cout << "Nut [" << i+1 << "]:";
        cin >> arr_of_nuts[i];
    }
    cout << "Done!\n";
}
void check_arrays(int count, int arr_of_bolts[], int arr_of_nuts[]){
    cout << "=== Check ===\n";
    cout << "Bolts:";
    for (int i = 0; i < count; i++)
        cout << arr_of_bolts[i] << " ";
    cout << endl;
    cout << "Nuts:";
    for (int i = 0; i < count; i++)
        cout << arr_of_nuts[i] << " ";
    cout << endl;
    cout << "=============\n";
}
int find_pos_of_el(int data, int count, int arr[]){
    for (int i = 0; i < count; i++){
        if (arr[i] == data)
            return i;
    }
    return -1;
}
void quicksort_for_two(int bolts[], int nuts[], int low, int high, int count){
    int i = low;
    int j = high;
    int pivot = nuts[find_pos_of_el(bolts[(i+j)/2], count, nuts)];

    while (i <= j){
        while (bolts[i] < pivot)
            i++;
        while (bolts[j] > pivot)
            j--;
        if (i <= j){
            std::swap(bolts[i], bolts[j]);
            i++;
            j--;
        }
        if (j > low)
            quicksort_for_two(bolts, nuts, low, j, count);
        if (i < high)
            quicksort_for_two(bolts, nuts, i, high, count);
    }
}


int main() {
    cout << "Enter countber of bolts and nuts:";
    int count;
    cin >> count;
    int bolts[count];
    int nuts[count];
    fill_arrays(count, bolts, nuts);
    quicksort_for_two(bolts, nuts, 0, count-1, count);
    quicksort_for_two(nuts, bolts, 0, count-1, count);
    check_arrays(count, bolts, nuts);
    return 0;
}