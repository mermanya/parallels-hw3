#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int agrc, char* argv[]) {
    int m, n, startTime;    // Размеры матрицы и переменная для замера времени
    double *A, *B;   // Исходная и Транспонированная матрицы
    int procNum;    // Количество процессов
    int procRank;   // Ранг процесса

    // Инициализация MPI
	MPI_Init(&agrc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    if (procRank == 0) {
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
    }

    startTime = MPI_Wtime();

	// Рассылка полученных значений на все процессы
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(A, m * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int chunk = m / procNum;
	int begin = chunk * procRank;
	int end = chunk * (procRank + 1);

    if (procRank == procNum-1) {
        end = m;
    }

    for(int i = begin; i < end; i++) {
        for(int j = 0; j < n; j++) {
            B[i + j * n] = A[i * n + j];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
	// Вывод результатов
	if (procRank == 0) {
		printf("Параллельное транспанирование матриц (MPI): %d-process.\nВремя выполнения: %5.5f s\n", procNum, MPI_Wtime()-startTime);
    }

    // Завершение MPI
	MPI_Finalize();

	return(0);
}