#include<string>
#include<vector>
#include<stdio.h>
#include<omp.h>
#include "methods.h"
using namespace std;

void main() {

	const int noCores = omp_get_num_procs();
	printf("Number of cores: %d \n", noCores);

	const long N = 1e5;

	benchmark("Sequential solution", N, sequentialSolution);
	benchmark("Parallel solution with race condition on result", N, parallelRaceSolution);
	benchmark("Parallel solution with mutex", N, parallelMutexSolution);
	benchmark("Parallel solution with a mutex and a better load balancing", N, parallelLoadBalancingSolution);
	//benchmark("Parallel solution with a mutex and an optimized load balancing", N, parallelBetterLoadBalancingSolution);
	//benchmark("Parallel solution without a mutex and an optimized load balancing", N, parallelBetterLoadBalancingSolutionWithoutMutex);
	//benchmark("Parallel solution with omp", N, ompParallelSolution);
}
