#include "includes/main.h"

int **alocArray(int n, int m) {
	int **array = calloc(n,sizeof(int*));
	if (array == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	for (int i=0; i < n; i++) {
		array[i] = calloc(m,sizeof(int));
		if (array[i] == NULL) {
			perror(PROGRAM);
			exit(EXIT_FAILURE);
		}
	}
	return array;
}

void iniGraph(int **array) {
	int i;
	for (int r=0; r < WROW; r++) {
		for (int c=0; c < WCOL; c++) {
			i = r * WCOL + c;
			if (c > 0) // Two inner diagonals
				array[i-1][i] = array[i][i-1] = 1;
			if (r > 0) // Two outer diagonals
				array[i-WCOL][i] = array[i][i-WCOL] = 1;
		}
	}
}

void setVgrau(int **array, Room *room) {
	for (int i=0; i < ROOM; i++)
		for (int j=0; j < ROOM; j++)
			if (array[i][j])
				room[i].grau++;
}

int randonum(int low, int high) {
	double d = (double)rand() / ((double)RAND_MAX + 1);
	int k = d * (high - low + 1);
	return low + k;
}

void freeArray(int n, int **ptr) {
	for (int i=0 ; i<n ; i++)
		free(ptr[i]);
	free(ptr);
}