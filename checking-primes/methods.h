#pragma once
#include<string>
#include<vector>
#include<stdio.h>
#include<omp.h>
#include<thread>
#include<mutex>
using namespace std;

void countPrimes(long lowerLimit, long upperLimit, long& result) {
	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}
}

// Count number of primes in a given set of numbers.
long sequentialSolution(long setSize) {
	long noPrimes = 0;
	countPrimes(0, setSize, noPrimes);
	return noPrimes;
}

// Parallel solution with race condition on result
long parallelRaceSolution(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();

	vector<thread> threads;
	long intervalSize = setSize / noThreads;

	for (int i = 0; i < noThreads; ++i) {
		long lowerLimit = i * intervalSize;
		long upperLimit = (i + 1) * intervalSize;
		if (i == noThreads - 1) {
			upperLimit = setSize;
		}

		threads.push_back(thread(countPrimes, lowerLimit, upperLimit, ref(noPrimes)));
	}

	for (int i = 0; i < noThreads; ++i) {
		threads[i].join();
	}

	return noPrimes;
}

// Parallel solution with mutex
void syncCountPrimes(long lowerLimit, long upperLimit, long& result, mutex& mutex) {

	double tStart = omp_get_wtime();

	for (long i = lowerLimit; i < upperLimit; ++i) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}

	double tFinal = omp_get_wtime();
	printf("	>> Thread duration = %f seconds \n", tFinal - tStart);
}

long parallelMutexSolution(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	mutex mutex;

	vector<thread> threads;
	long intervalSize = setSize / noThreads;

	for (int i = 0; i < noThreads; ++i) {
		long lowerLimit = i * intervalSize;
		long upperLimit = (i + 1) * intervalSize;
		if (i == noThreads - 1) {
			upperLimit = setSize;
		}

		threads.push_back(thread(syncCountPrimes, lowerLimit, upperLimit, ref(noPrimes), ref(mutex)));
	}

	for (int i = 0; i < noThreads; ++i) {
		threads[i].join();
	}

	return noPrimes;
}

// Parallel solution with a mutex and a better load balancing
void syncVerifyPrime(long value, long& result, mutex& mutex) {
	bool isPrime = true;
	for (long j = 2; j < value / 2; ++j) {
		if (value % j == 0) {
			isPrime = false;
			break;
		}
	}

	mutex.lock();
	if (isPrime) {
		result += 1;
	}
	mutex.unlock();
}

void syncCountPrimesWithGivenStep(long lowerLimit, long upperLimit, long stepSize, long& result, mutex& mutex) {

	double tStart = omp_get_wtime();

	for (long i = lowerLimit; i < upperLimit; i += stepSize) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		mutex.lock();
		if (isPrime) {
			result += 1;
		}
		mutex.unlock();
	}

	double tFinal = omp_get_wtime();
	printf("	>> Thread duration = %f seconds \n", tFinal - tStart);
}

long parallelLoadBalancingSolution(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	mutex mutex;
	vector<thread> threads;
	long intervalSize = setSize / noThreads;

	for (int i = 0; i < noThreads; ++i) {
		threads.push_back((thread(syncCountPrimesWithGivenStep, i, setSize, noThreads, ref(noPrimes), ref(mutex))));
	}
	for (int i = 0; i < noThreads; ++i) {
		threads[i].join();
	}
	return noPrimes;
}

// Parallel solution with a mutex and an optimized load balancing
long parallelOptimizedLoadBalancingSolution(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	mutex mutex;
	vector<thread> threads;
	long intervalSize = setSize / noThreads;

	// Here we exclude the even values - they cannot be prime.
	long startingValue = 1;

	for (int i = 0; i < noThreads; ++i) {
		threads.push_back((thread(syncCountPrimesWithGivenStep, startingValue, setSize, noThreads * 2, ref(noPrimes), ref(mutex))));
		startingValue += 2;
	}
	for (int i = 0; i < noThreads; ++i) {
		threads[i].join();
	}

	// Add 0, 1 and 2, because we start counting at 1.
	noPrimes += 3;

	return noPrimes;
}

// Parallel solution with a mutex and an optimized load balancing
void countPrimesWithGivenStep(long lowerLimit, long upperLimit, long stepSize, long& result) {
	double tStart = omp_get_wtime();

	for (long i = lowerLimit; i < upperLimit; i += stepSize) {
		bool isPrime = true;
		for (long j = 2; j < i / 2; ++j) {
			if (i % j == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			result += 1;
		}
	}

	double tFinal = omp_get_wtime();
	printf("	>> Thread duration = %f seconds \n", tFinal - tStart);
}

// Parallel solution with omp


long parallelOptimizedLoadBalancingSolutionWithoutMutex(long setSize) {
	long noPrimes = 0;
	int noThreads = omp_get_num_procs();
	vector<thread> threads;
	long intervalSize = setSize / noThreads;

	long** results = new long* [noThreads];
	for (int i = 0; i < noThreads; i++) {
		results[i] = new long[1];
		results[i][0] = 0;
	}

	// Here we exclude the even values - they cannot be prime.
	long startingValue = 1;

	for (int i = 0; i < noThreads; ++i) {
		threads.push_back((thread(countPrimesWithGivenStep, startingValue, setSize, noThreads * 2, ref(results[i][0]))));
		startingValue += 2;
	}
	
	for (int i = 0; i < noThreads; ++i) {
		threads[i].join();
	}

	for (int i = 0; i < noThreads; ++i) {
		noPrimes += results[i][0];
	}

	// Add 0, 1 and 2, because we start counting at 1.
	noPrimes += 3;

	return noPrimes;
}

void benchmark(string description,
	long setSize,
	long (*functionPointer)(long)) {

	printf("Benchmark Test for %s \n", description.c_str());

	double tStart = omp_get_wtime();
	long result = functionPointer(setSize);
	double tFinal = omp_get_wtime();

	printf(" | Result is: %d \n", result);
	printf(" | Required time: %f s \n", tFinal - tStart);
}