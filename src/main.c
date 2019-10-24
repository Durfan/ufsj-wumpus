#include "includes/main.h"

int **ini2Darray(void) {
	int **array = calloc(WORLDSIZE,sizeof(int*));
	if (array == NULL)
		exit(EXIT_FAILURE);
	for (int i=0; i < WORLDSIZE; i++) {
		array[i] = calloc(WORLDSIZE,sizeof(int));
		if (array[i] == NULL)
			exit(EXIT_FAILURE);
	}
	return array;
}

void freeArray(int **ptr) {
	for (int i=0 ; i < WORLDSIZE ; i++)
		free(ptr[i]);
	free(ptr);
}

void prtArray(int **array) {
	for (int i=0; i < WORLDSIZE; i++) {
		for (int j=0; j < WORLDSIZE; j++)
			printf(" %d", array[i][j]);
		putchar(0x0A);
	}
}

int limits(int n) {
	if (n >= WORLDSIZE || n < 0)
		return 1;
	return 0;
}

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));

	int **world = ini2Darray();

	int r;

	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);
	r = rand() % WORLDSIZE;
	printf("teste--> %d\n", r);

	prtArray(world);

	freeArray(world);
	return EXIT_SUCCESS;
}