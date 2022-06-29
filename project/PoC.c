#define _CRT_SECURE_NO_WARNINGS
#define ERROR_CODE_WRONG_PASSWORD 20992
#define ERROR_CODE_BROKEN_FILE 512
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<mpi.h>
#include<omp.h>

// P09s

void generateChars(char chars[]) {
	chars[0] = '0';
	chars[1] = 'P';
	chars[2] = 's';
	chars[3] = '9';
}

void crack(const char* archive, const char* archiveFile, int world_rank, int world_size) {

	const int arraySize = 4;
	char* chars = malloc(arraySize * sizeof(char));
	generateChars(chars);

	char unzip[] = "unzip -qo -P ____ ";
	char devnull[] = " 2>/dev/null";

	char* command = (char*)malloc(sizeof(char) * (strlen(unzip) + strlen(archive) + strlen(devnull) + 1));
	strcpy(command, unzip);
	strcat(command, archive);
	strcat(command, devnull);

	FILE* file;
	volatile bool flag = false;

	int max = arraySize;
	int setSize = max / world_size;

	int lowerLimit = 0, upperLimit = 0;

	if (world_rank == 0) lowerLimit = setSize * world_rank;
	else lowerLimit = setSize * world_rank + 1;

	if (world_rank == world_size - 1) upperLimit = max;
	else upperLimit = setSize * (world_rank + 1) + 1;

#pragma omp for 
	for (int i = lowerLimit; i < upperLimit; i++) {
		if (flag) continue;
		for (int j = 0; j < arraySize; j++) {
			if (flag) continue;
			for (int k = 0; k < arraySize; k++) {
				if (flag) continue;
				for (int l = 0; l < arraySize; l++) {

					command[13] = chars[i];
					command[14] = chars[j];
					command[15] = chars[k];
					command[16] = chars[l];

					printf("Rank: %d | Command: %s\n", world_rank, command);
					int code = system(command);
					if (code != ERROR_CODE_WRONG_PASSWORD && code != ERROR_CODE_BROKEN_FILE) {
						if (file = fopen(archiveFile, "r")) {

							char str[100];
							if (fgets(str, 100, file) != NULL) {
								printf("Password: %c%c%c%c \n", chars[i], chars[j], chars[k], chars[l]);
								printf("Text from file:\n %s \n\n", str);
								flag = true;
							}

							fclose(file);
							MPI_Abort(MPI_COMM_WORLD, 0);
						}
					}
				}
			}
		}
	}
}

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	double tStart = omp_get_wtime();

	if (argv[1] != NULL && argv[2] != NULL) {
		crack(argv[1], argv[2], world_rank, world_size);
	}
	else {
		const char* archive = "protected.zip";
		const char* archiveFile = "test.txt";
		crack(archive, archiveFile, world_rank, world_size);
	}

	double tFinal = omp_get_wtime();
	printf(" | Required time: %f s \n", tFinal - tStart);

	MPI_Finalize();

	return 0;
}
