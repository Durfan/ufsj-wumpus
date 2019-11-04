#include "main.h"

Quad *iniQuad(void) {
	Quad *quad = malloc(QUAD * sizeof(Quad));
	if (quad == NULL) {
		perror(PROJECT_NAME);
		exit(EXIT_FAILURE);
	}

	for (int i=0; i < QUAD; i++) {
		quad[i].ghost = false;
		quad[i].smell = false;
		quad[i].traps = false;
		quad[i].wind  = false;
		quad[i].gold  = false;
	}

	return quad;
}

Know *iniKnow(void) {
	Know *quad = malloc(QUAD * sizeof(Know));
	if (quad == NULL) {
		perror(PROJECT_NAME);
		exit(EXIT_FAILURE);
	}

	for (int i=0; i < QUAD; i++) {
		quad[i].visit = false;
		quad[i].paths = 0;
		quad[i].ghost = noinf;
		quad[i].smell = false;
		quad[i].traps = noinf;
		quad[i].wind  = false;
		quad[i].gold  = false;
	}

	return quad;
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

void genWorld(int **world, Quad *wquad) {
	int qty,trap,gold,ghost;
	List *exclude = iniLst();
	pshLst(exclude,0);
	pshLst(exclude,1);
	pshLst(exclude,WCOL);
	pshLst(exclude,WCOL+1);

	qty = 0;
	while (qty != 1) {
		ghost = GetRandomValue(0,QUAD-1);
		if (!excluded(exclude,ghost)) {
			wquad[ghost].ghost = true;
			for (int i=0; i < QUAD; i++)
				if (world[ghost][i])
					wquad[i].smell = true;
			qty++;
		}
	}

	qty = 0;
	while (qty != 3) {
		trap = GetRandomValue(0,QUAD-1);
		if (!excluded(exclude,trap)) {
			wquad[trap].traps = true;
			for (int i=0; i < QUAD; i++)
				if (world[trap][i])
					wquad[i].wind = true;
			pshLst(exclude,trap);
			qty++;
		}
	}

	qty = 0;
	while (qty != 1) {
		gold = GetRandomValue(0,QUAD-1);
		if (!excluded(exclude,gold)) {
			wquad[gold].gold = true;
			qty++;
		}
	}

	#ifdef DEBUG
	prtWorld(wquad);
	prtLst(exclude);
	#endif

	clrLst(exclude);
}

void prtwchar(bool cond, char *str) {
	if (cond)
		printf("%s", str);
	else
		printf("\u2500");
}

void prtWorld(Quad *quad) {
	int barran = 0;
	for (int i=0; i < QUAD; i++) {
		prtwchar(quad[i].ghost,"W");
		prtwchar(quad[i].smell,"F");
		prtwchar(quad[i].traps,"P");
		prtwchar(quad[i].wind, "B");
		prtwchar(quad[i].gold, "G");

		barran++;
		if (barran == WCOL) {
			barran = 0;
			putchar(0x0A);
		}
	}
}

void prtGraph(int **array) {
	for (int i=0; i < QUAD; i++) {
		printf(" %d:", i+1);
		for (int j=0; j < QUAD; j++)
			printf(" %d", array[i][j]);
		printf(" : grau %d", getVgrau(array,i));
		putchar(0x0A);
	}
}

void prtAdjac(int **array) {
	for (int i=0; i < QUAD; i++) {
		printf(" %d: ", i);
		for (int j=0; j < QUAD; j++)
			if (array[i][j])
				printf(" %d", j);
		putchar(0x0A);
	}
}

void rstWorld(int **world, int **know,
	Agent *agent, Quad *wquad, Know *aquad) {

	agent->coord = START;
	agent->lives = true;
	agent->score = 0;
	agent->ghost = 0;
	agent->arrow = 1;
	agent->grito = false;
	agent->limit = false;

	for (int i=0; i < QUAD; i++) {
		wquad[i].ghost = false;
		wquad[i].smell = false;
		wquad[i].traps = false;
		wquad[i].wind  = false;
		wquad[i].gold  = false;
		aquad[i].visit = false;
		aquad[i].paths = 0;
		aquad[i].ghost = noinf;
		aquad[i].smell = false;
		aquad[i].traps = noinf;
		aquad[i].wind  = false;
		aquad[i].gold  = false;
	}

	for (int i=0; i < QUAD; i++)
		for (int j=0; j < QUAD; j++)
			know[i][j] = 0;
	
	genWorld(world,wquad);
}