#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const long long N = 100000000;
    double* arr = new double[N];

    srand(time(NULL));
    for (long long i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }

    // Последовательный вариант
    double start = omp_get_wtime();
    double sum_seq = 0;
    for (long long i = 0; i < N; i++) {
        sum_seq += arr[i];
    }
    double end = omp_get_wtime();
    cout << "Consistently: Sum = " << sum_seq << ", Time = " << (end - start) << " s" << endl;

    // Параллельный вариант с разным числом потоков
    for (int threads : {1, 2, 4, 8}) {
        omp_set_num_threads(threads);
        double sum_par = 0;

        start = omp_get_wtime();
#pragma omp parallel for reduction(+:sum_par)
        for (long long i = 0; i < N; i++) {
            sum_par += arr[i];
        }
        end = omp_get_wtime();

        cout << "Threads " << threads << ": Sum = " << sum_par
            << ", Time = " << (end - start) << " s" << endl;
    }

    delete[] arr;
    return 0;
}
