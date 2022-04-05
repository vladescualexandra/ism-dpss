#include<thread>
#include<string>
#include<vector>
#include<omp.h>
#include<mutex>
using namespace std;

/*
* OpenMPI connects directly to the system and to the kernel.
* So it has the power to access some resources that we couldn't
* access otherwise.
*/

// Define a global mutex.
mutex counterMutex;

void counter(int& counter, int limit) {
	for (int i = 0; i < limit; i++) {
		counter += 1;
	}
}

void counterWithMutex(int& counter, int limit) {
	for (int i = 0; i < limit; i++) {
		// This will result in sequentiality.
		counterMutex.lock();
		counter += 1;
		counterMutex.unlock();
	}
}

void main() {

	// Basic functions from OpenMPI
	// 
	// 1. Get number of processors.
	int noCores = omp_get_num_procs();
	printf("1. Available cores: %d \n", noCores);

	// 2. Get current time.
	double currentTime = omp_get_wtime();
	printf("2. Current time: %f s \n", currentTime);

	// 3. Cache coherence problem - basic example.
	int finalCounter = 0;
	vector<thread> threads;
	const int limit = 1e6;
	
	double tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counter, ref(finalCounter), limit));
	}
	
	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}

	double tFinal = omp_get_wtime();

	printf("3. The counter result with race condition and multiple threads: %f s \n",
		tFinal - tStart);

	// 4. Threads with mutex.
	threads.clear();
	finalCounter = 0;

	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counterWithMutex, ref(finalCounter), limit));
	}

	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}

	tFinal = omp_get_wtime();

	printf("4. The counter result with mutex: %f s \n",
		tFinal - tStart);

	// 5. Testing without mutex and race condition, with multiple variables.
	threads.clear();
	finalCounter = 0;
	int* threadResults = new int[noCores];
	for (int i = 0; i < noCores; i++) {
		threadResults[i] = 0;
	}

	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counter, ref(threadResults[i]), limit));
	}

	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}

	tFinal = omp_get_wtime();

	for (int i = 0; i < noCores; i++) {
		finalCounter += threadResults[i];
	}

	printf("5. The counter result without mutex and race condition, with multiple variables: %f s \n",
		tFinal - tStart);

	// 6. Testing without mutex and race condition, 
	// with multiple variables on different cache lines.
	// It means that if you need a value from a vector, 
	// the entire vector will be taken to the cache line.
	// So if if you have different cache lines, it will
	// create a hardware mutex to keep them synchronized.
	threads.clear();
	finalCounter = 0;
	int threadResultsMatrix[8][1000]; // noCores = 8
	// Use a matrix so the values are apart from eachoter, 
	// we only need the first column.
	for (int i = 0; i < noCores; i++) {
		threadResultsMatrix[i][0] = 0;
	}

	

	tStart = omp_get_wtime();
	for (int i = 0; i < noCores; i++) {
		threads.push_back(thread(counter, ref(threadResultsMatrix[i][0]), limit));
	}

	for (int i = 0; i < noCores; i++) {
		threads[i].join();
	}

	tFinal = omp_get_wtime();

	for (int i = 0; i < noCores; i++) {
		finalCounter += threadResultsMatrix[i][0];
	}

	printf("6. The counter result without cache coherence: %f s \n",
		tFinal - tStart);
}