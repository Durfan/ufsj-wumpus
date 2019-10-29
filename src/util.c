#include "includes/main.h"

void iniStructs(void) {
	memset(g_world,0,sizeof g_world);
	memset(g_knowB,0,sizeof g_knowB);

	int i;
	for (int r=0; r < WSIZE; r++) {
		for (int c=0; c < WSIZE; c++) {
			i = r * WSIZE + c;
			if (c > 0) 
				g_world[i-1][i] = g_world[i][i-1] = 1;
			if (r > 0)
				g_world[i-WSIZE][i] = g_world[i][i-WSIZE] = 1;
		}
	}

}

int rndInt(int low, int high) {
	double d = (double)rand() / ((double)RAND_MAX + 1);
	int k = d * (high - low + 1);
	return low + k;
}