#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<time.h>

void main() {

	float startTime = (float)clock() / CLOCKS_PER_SEC;

	char characters[62];
	// numbers from 0 to 9 = 10		-|
	// small letters	   = 26		 | = 62 possible characters
	// capital letters     = 26		-|

	char command[] = "unzip -qo -P ____ protected.zip 2>/dev/null";

	int counter = 0;
	char index;
	for (index = '0'; index <= '9'; ++index) {
		characters[counter] = index;
		counter++;
	}

	for (index = 'a'; index <= 'z'; ++index) {
		characters[counter] = index;
		counter++;
	}

	for (index = 'A'; index <= 'Z'; ++index) {
		characters[counter] = index;
		counter++;
	}

	FILE* file;
	const char* fileName = "test.txt";
	bool isFound = false;
	for (int i = 0; i < 62; i++) {
		if (isFound) break;
		for (int j = 0; j < 62; j++) {
			if (isFound) break;
			for (int k = 0; k < 62; k++) {
				if (isFound) break;
				for (int l = 0; l < 62; l++) {

					command[13] = characters[i];
					command[14] = characters[j];
					command[15] = characters[k];
					command[16] = characters[l];

					system(command);

					if (file = fopen(fileName, "r")) {

						char str[100];
						if (fgets(str, 100, file) != NULL) {
							printf("Command: %s \n", command);
							printf("Text from file: %s\n", str);
							isFound = true;
						}

						fclose(file);
					}
				}
			}
		}
	}

	float endTime = (float)clock() / CLOCKS_PER_SEC;
	float timeElapsed = endTime - startTime;

	printf("Time elapsed: %.6f", timeElapsed);
}