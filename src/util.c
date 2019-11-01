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

int getVgrau(int **array, int v) {
	int grau = 0;
	for (int i=0; i < QUAD; i++)
		if (array[v][i])
			grau++;
	return grau;
}

void freeArray(int n, int **ptr) {
	for (int i=0 ; i<n ; i++)
		free(ptr[i]);
	free(ptr);
}