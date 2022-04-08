#include<string>
#include<omp.h>
using namespace std;

void main() {

	long sum = 0;
	const int N = 10000;
	int* array = new int[N];
	for (int i = 0; i < N; i++) {
		array[i] = i + 1;
	}

	// Standard solution.
	for (int i = 0; i < N; i++) {
		sum += array[i];
	}
	printf("Standard solution: sum = %d \n", sum);

	// Parallel solution with shared sum and race condition.
	sum = 0;
#pragma omp parallel
	{
		for (int i = 0; i < N; i++) {
			sum += array[i];
		}
	}
	printf("Parallel solution with shared sum and race condition: sum = %d \n", sum);


	// Parallel solution with OMP mutex.
	// The for is duplicated on all threads.
	sum = 0;
#pragma omp parallel
	{
		for (int i = 0; i < N; i++) {
#pragma omp critical
			sum += array[i];
		}
	}

	printf("Parallel solution with OMP mutex: sum = %d \n", sum);

	// Parallel with work sharing of the for loop.
	sum = 0;
#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < N; i++) {
			omp_set_lock(&mutex);
			sum += array[i];
			omp_unset_lock(&mutex);
		}
	}

	printf("Parallel with work sharing of the for loop: sum = %d \n", sum);

	// Parallel with local variables and reduction.
	sum = 0;
#pragma omp parallel for reduction(+: sum)
	for (int i = 0; i < N; i++) {
		sum += array[i];
	}

	printf("Parallel with local variables and reduction: sum = %d \n", sum);


	sum = 0;
#pragma omp parallel 
	{
#pragma omp for reduction(+: sum) schedule(dynamic, 10)
		for (int i = 0; i < N; i++) {
			printf("Thread id %d got iteration %d \n", omp_get_thread_num(), i);
			sum += array[i];
		}
	}


	sum = 0;
#pragma omp parallel 
	{
#pragma omp for reduction(+: sum) schedule(static)
		for (int i = 0; i < 40; i++) {
			printf(">>> Thread id %d got iteration %d \n", omp_get_thread_num(), i);
			sum += array[i];
		}
	}
}


