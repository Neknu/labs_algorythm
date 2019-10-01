// C++ program to demonstrate all operations of 
// k-ary Heap 
#include<bits/stdc++.h>

using namespace std;

void restoreDown(int arr[], int len, int index,
                 int k)
{
    int child[k+1];

    while (true)
    {

        for (int i=1; i<=k; i++)
            child[i] = ((k*index + i) < len) ?
                       (k*index + i) : -1;

        int max_child = -1, max_child_index ;

        for (int i=1; i<=k; i++)
        {
            if (child[i] != -1 &&
                arr[child[i]] > max_child)
            {
                max_child_index = child[i];
                max_child = arr[child[i]];
            }
        }

        // leaf node 
        if (max_child == -1)
            break;

        if (arr[index] < arr[max_child_index])
            swap(arr[index], arr[max_child_index]);

        index = max_child_index;
    }
}

// Restores a given node up in the heap.
void restoreUp(int arr[], int index, int k)
{
    int parent = (index-1)/k;

    while (parent>=0)
    {
        if (arr[index] > arr[parent])
        {
            swap(arr[index], arr[parent]);
            index = parent;
            parent = (index -1)/k;
        }

            // node has been restored at the correct position
        else
            break;
    }
}

// Function to build a heap of arr[0..n-1] and value of k.
void buildHeap(int arr[], int n, int k)
{
    for (int i= (n-1)/k; i>=0; i--) // here (n-2) / k  is also posible
        restoreDown(arr, n, i, k);
}

void insert(int arr[], int* n, int k, int elem)
{
    arr[*n] = elem;

    *n = *n+1;

    restoreUp(arr, *n-1, k);
}

int extractMax(int arr[], int* n, int k)
{
    int max = arr[0];

    arr[0] = arr[*n-1];

    *n = *n-1;

    restoreDown(arr, *n, 0, k);

    return max;
}

void increaseKey(int arr[], int pos, int k, int elem) {
    if(elem <= arr[pos])
        return;
    arr[pos] = elem;

    restoreUp(arr, pos, k);
}


int main()
{
    const int capacity = 100;
    int arr[capacity] = {4, 5, 6, 7, 8, 9, 10};
    int n = 7;
    int k = 3;

    buildHeap(arr, n, k);

    printf("Built Heap : \n");
    for (int i=0; i<n; i++)
        printf("%d ", arr[i]);

    increaseKey(arr, 2, k, 12);

    printf("\n\nIncreaseKey Heap : \n");
    for (int i=0; i<n; i++)
        printf("%d ", arr[i]);

    int element = 3;
    insert(arr, &n, k, element);

    printf("\n\nHeap after insertion of %d: \n",
           element);
    for (int i=0; i<n; i++)
        printf("%d ", arr[i]);

    printf("\n\nExtracted max is %d",
           extractMax(arr, &n, k));

    printf("\n\nHeap after extract max: \n");
    for (int i=0; i<n; i++)
        printf("%d ", arr[i]);

    return 0;
} 