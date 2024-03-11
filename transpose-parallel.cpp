#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void transpose(int m, int n, double *A, double *B);

int main(int agrc, char* argv[]) {
    int m, n, startTime;    // Размеры матрицы и переменная для замера времени
    double *A, *B;   // Исходная и Транспонированная матрицы

    printf("Введите размеры матрицы (m, n): ");
    scanf("%d%d", &m, &n);
    printf("\n");

    // Выделение памяти под элементы матриц
    A = (double *) malloc(m * n * sizeof(double));
    B = (double *) malloc(m * n * sizeof(double));

    printf("Введите элементы матрицы (%d, %d):\n", m, n);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            // Ввод элементов с индексами i, j
            scanf("%lf ", &(A[i * n + j]));
        }
    }
    printf("\n\n");
    
    startTime = omp_get_wtime();

    transpose(m, n, A, B);

	printf("Транспанирование матрицы (распараллеливание).\nВремя выполнения: %5.5f s\n", omp_get_wtime()-startTime);

	return(0);
}


void transpose(int m, int n, double *A, double *B) {
    omp_set_num_threads(4);
    #pragma omp parallel private (m, n, A, B) num_threads(4)
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            B[i + j * m] = A[i * n + j];
        }
    }
}