#include "includes/main.h"

void genWorld(void) {
	int wumpus = rndInt(0,ROOMS-1);
	g_wroom[wumpus].wumpus = true;
	for (int i=0; i < ROOMS; i++)
		if (g_world[wumpus][i])
			g_wroom[i].fedor = true;
}

void prtWorld(void) {
	int barran = 0;
	for (int i=0; i < ROOMS; i++) {
		if (g_wroom[i].wumpus)
			printf(" W");
		else if (g_wroom[i].fedor)
			printf(" F");
		else
			printf(" 0");
		
		barran++;
		if (barran == WCOLS) {
			barran = 0;
			putchar(0x0A);
		}
	}
}

void prtGraph(void) {
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