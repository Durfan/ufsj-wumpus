#include "includes/main.h"

void genWorld(void) {
	int x,y;

	x = rndInt(0,3);
	y = rndInt(0,3);
	g_world[x][y] = 1;
}

void prtWorld(void) {
	for (int i=0; i < ROOMS; i++) {
		for (int j=0; j < ROOMS; j++)
			printf(" %d", g_world[i][j]);
		putchar(0x0A);
	}
}

void prtAdjac(void) {
	for (int i=0; i < ROOMS; i++) {
		printf(" %d: ", i);
		for (int j=0; j < ROOMS; j++)
			if (g_world[i][j])
				printf(" %d", j);
		putchar(0x0A);
	}
}