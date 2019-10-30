#include "includes/main.h"

Room *initRoom(void) {
	Room *room = malloc(ROOM * sizeof(Room));
	if (room == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	for (int i=0; i < ROOM; i++) {
		room[i].wumpus = false;
		room[i].fedor  = false;
		room[i].whell  = false;
		room[i].brisa  = false;
		room[i].gold   = false;
		room[i].reluz  = false;
		room[i].limit  = false;
	}

	return room;
}

void genWorld(int **world,Room *wroom) {
	int whell = 0;

	while (whell != 3) {

	}

	int wumpus = randonum(0,ROOM-1);
	wroom[wumpus].wumpus = true;
	for (int i=0; i < ROOM; i++)
		if (world[wumpus][i])
			wroom[i].fedor = true;
}

void prtWorld(Room *room) {
	int barran = 0;
	for (int i=0; i < ROOM; i++) {
		if (room[i].wumpus)
			printf(" W");
		else if (room[i].fedor)
			printf(" F");
		else
			printf(" 0");
		
		barran++;
		if (barran == WCOL) {
			barran = 0;
			putchar(0x0A);
		}
	}
}

void prtGraph(int **array) {
	for (int i=0; i < ROOM; i++) {
		for (int j=0; j < ROOM; j++)
			printf(" %d", array[i][j]);
		putchar(0x0A);
	}
}

void prtAdjac(int **array) {
	for (int i=0; i < ROOM; i++) {
		printf(" %d: ", i);
		for (int j=0; j < ROOM; j++)
			if (array[i][j])
				printf(" %d", j);
		putchar(0x0A);
	}
}