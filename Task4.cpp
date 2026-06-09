#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const int M = 1000;
    const int K = 1000;
    const int N = 1000;

    double** A = new double* [M];
    double** B = new double* [K];
    double** C = new double* [M];

    for (int i = 0; i < M; i++) {
        A[i] = new double[K];
        C[i] = new double[N];
    }
    for (int i = 0; i < K; i++) {
        B[i] = new double[N];
    }

    srand(time(NULL));
    for (int i = 0; i < M; i++)
        for (int j = 0; j < K; j++)
            A[i][j] = rand() % 10;

    for (int i = 0; i < K; i++)
        for (int j = 0; j < N; j++)
            B[i][j] = rand() % 10;

    // Последовательное умножение
    double start = omp_get_wtime();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Consistently: Time = " << (end - start) << "s" << endl;

    // Параллельное умножение (по внешнему циклу)
    for (int threads : {1, 2, 4, 8}) {
        omp_set_num_threads(threads);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                C[i][j] = 0;

        double start_par = omp_get_wtime();
#pragma omp parallel for
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0;
                for (int k = 0; k < K; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }
        double end_par = omp_get_wtime();

        cout << "Threads " << threads << ": Time = " << (end_par - start_par) << "s" << endl;
    }

    for (int i = 0; i < M; i++) {
        delete[] A[i];
        delete[] C[i];
    }
    for (int i = 0; i < K; i++) {
        delete[] B[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
