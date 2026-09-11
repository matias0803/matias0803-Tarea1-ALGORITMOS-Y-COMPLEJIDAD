//fuente: https://www.geeksforgeeks.org/dsa/merge-sort/
#include <iostream>
#include <vector>
using namespace std;

void merge(vector<int>& arr, int izq, int der, int mid) {

    int num1 = mid - izq + 1;
    int num2 = der - mid;

    vector<int> arrIzq(num1), arrDer(num2);

    for (int i = 0; i < num1; i++)
    {
        arrIzq[i] = arr[izq + i];
    }

    for (int j = 0; j < num2; j++)
    {
        arrDer[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;

    int k = izq;

    while (i < num1 && j < num2) {

        if (arrIzq[i] <= arrDer[j])
        {
            arr[k] = arrIzq[i];
            i++;
        }
        else {
            arr[k] = arrDer[j];
            j++;
        }

        k++;
    }

    while (i < num1) {
        arr[k] = arrIzq[i];
        i++;
        k++;
    }

    while (j < num2) {
        arr[k] = arrDer[j];
        j++;
        k++;
    }
}


void mergeSort(vector<int>& arr, int izq, int der) {

    if (izq >= der)
    {
        return;
    }
    int mid = izq + (der - izq) / 2;
    mergeSort(arr, izq, mid);
    mergeSort(arr, mid + 1, der);
    merge(arr, izq, der, mid);
}




void mergeSort(std::vector<int>& arr) {
    if (!arr.empty()) mergeSort(arr, 0, arr.size() - 1);
}