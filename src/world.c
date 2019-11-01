#include "includes/main.h"

Room *initRoom(void) {
	Room *room = malloc(ROOM * sizeof(Room));
	if (room == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	for (int i=0; i < ROOM; i++) {
		room[i].id = i;
		room[i].grau = 0;
		room[i].ghost = false;
		room[i].smell = false;
		room[i].traps = false;
		room[i].wind  = false;
		room[i].gold  = false;
		room[i].lmit  = false;
	}

	return room;
}

Bknow *iniBknow(void) {
	Bknow *room = malloc(ROOM * sizeof(Bknow));
	if (room == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	for (int i=0; i < ROOM; i++) {
		room[i].id = i;
		room[i].grau = 0;
		room[i].visited = false;
		room[i].ghost = 0;
		room[i].traps = 0;
		room[i].smell = false;
		room[i].wind  = false;
		room[i].gold  = false;
		room[i].lmit  = false;
	}

	return room;
}

int excluded(List *list, int key) {
	if (lstnil(list)) return 0;
	Node *ptr = list->head;
	while (ptr != NULL) {
		if (ptr->key == key)
			return 1;
		ptr = ptr->next;
	}
	return 0;
}

void rstWorld(int **world, Room *room) {
	for (int i=0; i < ROOM; i++) {
		room[i].ghost = false;
		room[i].smell = false;
		room[i].traps = false;
		room[i].wind  = false;
		room[i].gold  = false;
		room[i].lmit  = false;
	}
	genWorld(world,room);
}

void genWorld(int **world, Room *wroom) {
	int qty,whell,gold,wumpus;
	List *exclude = iniLst();
	pshLst(exclude,0);
	pshLst(exclude,1);
	pshLst(exclude,WCOL);
	pshLst(exclude,WCOL+1);

	qty = 0;
	while (qty != 1) {
		wumpus = GetRandomValue(0,ROOM-1);
		if (!excluded(exclude,wumpus)) {
			wroom[wumpus].ghost = true;
			for (int i=0; i < ROOM; i++)
				if (world[wumpus][i])
					wroom[i].smell = true;
			qty++;
		}
	}

	qty = 0;
	while (qty != 3) {
		whell = GetRandomValue(0,ROOM-1);
		if (!excluded(exclude,whell)) {
			wroom[whell].traps = true;
			for (int i=0; i < ROOM; i++)
				if (world[whell][i])
					wroom[i].wind = true;
			pshLst(exclude,whell);
			qty++;
		}
	}

	qty = 0;
	while (qty != 1) {
		gold = GetRandomValue(0,ROOM-1);
		if (!excluded(exclude,gold)) {
			wroom[gold].gold = true;
			qty++;
		}
	}

	#ifdef DEBUG
	prtWorld(wroom);
	prtLst(exclude);
	#endif

	clrLst(exclude);
}

void prtwchar(bool cond, char *str) {
	switch (cond) {
	case true:
		printf("%s", str);
		break;
	
	default:
		printf("\u2500");
		break;
	}

}

void prtWorld(Room *room) {
	int barran = 0;
	for (int i=0; i < ROOM; i++) {
		prtwchar(room[i].ghost,CRED"W"CRSET);
		prtwchar(room[i].smell,CRED"F"CRSET);
		prtwchar(room[i].traps,CBLUE"P"CRSET);
		prtwchar(room[i].wind, CBLUE"B"CRSET);
		prtwchar(room[i].gold, CYELL"G"CRSET);
		printf(":%d ", room[i].grau);

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