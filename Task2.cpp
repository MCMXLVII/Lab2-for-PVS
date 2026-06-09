#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

void swap(int& a, int& b) {
    int t = a;
    a = b;
    b = t;
}

int partition(int* arr, int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[right]);
    return i + 1;
}

// Последовательная быстрая сортировка
void quick_sort_seq(int* arr, int left, int right) {
    if (left < right) {
        int pivot = partition(arr, left, right);
        quick_sort_seq(arr, left, pivot - 1);
        quick_sort_seq(arr, pivot + 1, right);
    }
}

// Параллельная быстрая сортировка
void quick_sort_par(int* arr, int left, int right) {
    if (left < right) {
        int pivot = partition(arr, left, right);
#pragma omp task
        quick_sort_par(arr, left, pivot - 1);
#pragma omp task
        quick_sort_par(arr, pivot + 1, right);
    }
}

int main() {
    const int N = 10000000;
    int* arr_seq = new int[N];
    int* arr_par = new int[N];

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        int val = rand() % N;
        arr_seq[i] = val;
        arr_par[i] = val;
    }

    // Последовательный запуск
    double start = omp_get_wtime();
    quick_sort_seq(arr_seq, 0, N - 1);
    double end = omp_get_wtime();
    cout << "Consistently: Time = " << (end - start) << "s" << endl;

    // Параллельный запуск с разным числом потоков
    for (int threads : {1, 2, 4, 8}) {
        for (int i = 0; i < N; i++) {
            arr_par[i] = arr_seq[i];
        }
        srand(42);
        for (int i = 0; i < N; i++) {
            arr_par[i] = rand() % N;
        }
        omp_set_num_threads(threads);
        double start_par = omp_get_wtime();

#pragma omp parallel
        {
#pragma omp single
            quick_sort_par(arr_par, 0, N - 1);
        }

        double end_par = omp_get_wtime();
        cout << "Threads " << threads << ": Time = " << (end_par - start_par) << "s" << endl;
    }

    delete[] arr_seq;
    delete[] arr_par;
    return 0;
}
