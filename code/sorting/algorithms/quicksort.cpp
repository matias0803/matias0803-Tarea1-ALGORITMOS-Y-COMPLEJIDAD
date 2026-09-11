// fuente: https://www.geeksforgeeks.org/dsa/quick-sort-algorithm/
#include <iostream>
#include <vector>

using namespace std;

int partition(vector<int>& arr, int low, int high) {
    swap(arr[low + (high - low) / 2], arr[high]);
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}


void quickSortRecursivo(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        
        quickSortRecursivo(arr, low, pi - 1);
        quickSortRecursivo(arr, pi + 1, high);
    }
}

void quickSort(std::vector<int>& arr) {
    if (!arr.empty()) {
        quickSortRecursivo(arr, 0, arr.size() - 1);
    }
}