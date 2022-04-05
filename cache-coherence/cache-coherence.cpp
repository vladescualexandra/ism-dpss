#include<thread>
#include<string>
#include<omp.h>
using namespace std;

/*
* OpenMPI connects directly to the system and to the kernel.
* So it has the power to access some resources that we couldn't
* access otherwise.
*/

void main() {

	// Basic functions from OpenMPI
	// 
	// 1. Get number of processors.
	int noCores = omp_get_num_procs();
	printf("Available cores: %d", noCores);
}