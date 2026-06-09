#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const long long N = 50000000;
    double* a = new double[N];
    double* b = new double[N];
    double* c = new double[N];
    double* d = new double[N];
    double* e = new double[N];
    double* f = new double[N];

    srand(time(NULL));
    for (long long i = 0; i < N; i++) {
        a[i] = rand() % 100;
        b[i] = rand() % 100 + 1;
    }

    // Последовательный вариант
    double start = omp_get_wtime();
    for (long long i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
        d[i] = a[i] - b[i];
        e[i] = a[i] * b[i];
        f[i] = a[i] / b[i];
    }
    double end = omp_get_wtime();
    cout << "Consistently: Time = " << (end - start) << "s" << endl;

    // Параллельный вариант с разным числом потоков
    for (int threads : {1, 2, 4, 8}) {
        omp_set_num_threads(threads);

        double start_par = omp_get_wtime();
#pragma omp parallel for
        for (long long i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
            d[i] = a[i] - b[i];
            e[i] = a[i] * b[i];
            f[i] = a[i] / b[i];
        }
        double end_par = omp_get_wtime();

        cout << "Threads " << threads << ": Time = " << (end_par - start_par) << "s" << endl;
    }

    delete[] a; delete[] b; delete[] c;
    delete[] d; delete[] e; delete[] f;
    return 0;
}
