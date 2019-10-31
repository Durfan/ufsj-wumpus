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
		room[i].wumpus = false;
		room[i].fedor  = false;
		room[i].whell  = false;
		room[i].brisa  = false;
		room[i].gold   = false;
		room[i].limit  = false;
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
		room[i].wumpus = false;
		room[i].fedor  = false;
		room[i].whell  = false;
		room[i].brisa  = false;
		room[i].gold   = false;
		room[i].limit  = false;
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
		wumpus = randonum(0,ROOM-1);
		if (!excluded(exclude,wumpus)) {
			wroom[wumpus].wumpus = true;
			for (int i=0; i < ROOM; i++)
				if (world[wumpus][i])
					wroom[i].fedor = true;
			qty++;
		}
	}

	qty = 0;
	while (qty != 3) {
		whell = randonum(0,ROOM-1);
		if (!excluded(exclude,whell)) {
			wroom[whell].whell = true;
			for (int i=0; i < ROOM; i++)
				if (world[whell][i])
					wroom[i].brisa = true;
			pshLst(exclude,whell);
			qty++;
		}
	}

	qty = 0;
	while (qty != 1) {
		gold = randonum(0,ROOM-1);
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
		prtwchar(room[i].wumpus,CRED"W"CRSET);
		prtwchar(room[i].fedor, CRED"F"CRSET);
		prtwchar(room[i].whell,CBLUE"P"CRSET);
		prtwchar(room[i].brisa,CBLUE"B"CRSET);
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