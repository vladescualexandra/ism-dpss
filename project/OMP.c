#define _CRT_SECURE_NO_WARNINGS
#define ERROR_CODE_WRONG_PASSWORD 20992
#define ERROR_CODE_BROKEN_FILE 512
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<omp.h>

void generateChars(char chars[]) {
	int counter = 0;
	char index;
	for (index = '0'; index <= '9'; ++index) {
		chars[counter] = index;
		counter++;
	}

	for (index = 'a'; index <= 'z'; ++index) {
		chars[counter] = index;
		counter++;
	}

	for (index = 'A'; index <= 'Z'; ++index) {
		chars[counter] = index;
		counter++;
	}
}

void crack(const char* archive, const char* archiveFile) {

	const int arraySize = 10 + 26 + 26;
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

#pragma omp for 
	for (int i = 0; i < arraySize; i++) {
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
						}
					}
				}
			}
		}
	}
}

int main(int argc, char** argv) {

	double tStart = omp_get_wtime();

	if (argv[1] != NULL && argv[2] != NULL) {
		crack(argv[1], argv[2]);
	}
	else {
		const char* archive = "protected.zip";
		const char* archiveFile = "test.txt";
		crack(archive, archiveFile);
	}

	double tFinal = omp_get_wtime();

	printf(" | Required time: %f s \n", tFinal - tStart);

	return 0;
}
