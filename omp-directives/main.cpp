#include<string>
#include<omp.h>


// Settings:
// 1. Open Solution Explorer
// 2. Right-Click on "omp-directives" (2nd level)
// 3. Configuration Properties -> C/C++ -> All Options
// 4. Search for "Open MP Support"
// 5. Click on the arrow and select "Yes (/openmp)"
// 6. Apply
// After that, those two functions from #pragma omp parallel will be called 8 times each (for all the processors).
// noCores will be the default number of threads.

int main() {

	printf("Start of OMP Examples. \n");
	printf("Main thread. Id: %d \n", omp_get_thread_num()); 

	int noCores = omp_get_num_procs();
	printf("Number of cores: %d", noCores);

	#pragma omp parallel
		{
			printf("1. Thread id: %d \n", omp_get_thread_num());
		}
	
	// If you want to set the number of threads:
	omp_set_num_threads(10);

	printf("--------------------------\n");
	#pragma omp parallel
		{
			printf("2. Thread id: %d \n", omp_get_thread_num());
		}


	printf("--------------------------\n");
	#pragma omp parallel num_threads(3)
		{
			printf("3. Thread id: %d \n", omp_get_thread_num());
		}


	int hugeNumberOfThreads = 1e5;
	omp_set_num_threads(hugeNumberOfThreads);
	printf("--------------------------\n");
	#pragma omp parallel
	{
		if (omp_get_thread_num() == 0) {
			int noRealThreads = omp_get_num_threads();
			printf("4. The number of threads in the parallel section: %d \n", noRealThreads);
		}

		#pragma omp master
			{
				int noRealThreads = omp_get_num_threads();
				printf("5. The number of threads in the parallel section: %d \n", noRealThreads);
			}
	}


	// Controlling the execution in parallel or not.
	bool doItInParallel = true;

	// Not the good version.
	if (doItInParallel) {
		#pragma omp parallel
			{

			}

	}
	else {
		// Sequentially.
	}

	// The good version. :)
	if (noCores < 16) {
		doItInParallel = false;
	}

	printf("--------------------------\n");
	#pragma omp parallel if(doItInParallel)
		{
			printf("6. Parallel section, doItInParallel = true. \n");
		}


	// Managing variables.
	// All external variables are shared by all thread => race condition!
	int counter = 0;
	int noIterations = 10;

	#pragma omp parallel num_threads(4)
		{
			for (int i = 0; i < noIterations; i++) {
				counter += 1;
			}

			printf("Counter value: %d \n", counter);
		}

	printf("--------------------------\n");
	printf("Final counter value: %d \n", counter);


	printf("--------------------------\n");
	#pragma omp parallel num_threads(4)
		{
			int localCounter = 0;
			for (int i = 0; i < noIterations; i++) {
				localCounter += 1;
			}

			printf("Local Counter value: %d \n", localCounter);
		}

	#pragma omp parallel num_threads(4) private(counter)
		{
			counter = 0;
			for (int i = 0; i < noIterations; i++) {
				counter += 1;
			}

			printf("Counter value: %d \n", counter);
		}


	counter = 1000;
	#pragma omp parallel num_threads(4) firstprivate(counter)
		{
			counter = 0;
			for (int i = 0; i < noIterations; i++) {
				counter += 1;
			}

			printf("Counter value: %d \n", counter);
		}

	printf("End of OMP Examples. \n");
}



