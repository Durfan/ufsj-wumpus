#include "includes/main.h"

Agent *iniAgent(void) {
	Agent *agent = malloc(sizeof(Agent));
	if (agent == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	agent->coord = START;
	agent->lives = true;
	agent->score = 0;
	agent->arrow = 1;
	agent->grito = false;
	agent->limit = false;

	return agent;
}

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know) {
	int coord = agent->coord;
	aquad[coord].visit = true;
	bool visited;

	if (agent->lives) {
		aquad[coord].traps = nope;
		aquad[coord].ghost = nope;
	}

	if (sensor.gold)
		aquad[coord].gold = true;
	
	if (sensor.smell) {
		aquad[coord].smell = true;
		for (int i=0; i < QUAD; i++) {
			visited = aquad[i].visit;
			if (know[coord][i] && !visited)
				aquad[i].ghost = talvez;
		}
	}
	else {
		aquad[coord].smell = false;
		for (int i=0; i < QUAD; i++) {
			if (know[coord][i])
				aquad[i].ghost = nope;
		}
	}

	if (sensor.wind) {
		aquad[coord].wind = true;
		for (int i=0; i < QUAD; i++) {
			visited = aquad[i].visit;
			if (know[coord][i] && !visited)
				aquad[i].traps = talvez;
		}
	}
	else {
		aquad[coord].wind = false;
		for (int i=0; i < QUAD; i++) {
			if (know[coord][i])
				aquad[i].traps = nope;
		}
	}

	//confirm(aquad,know);

}

void confirm(Know *aquad, int **know) {
	for (int i=0; i < QUAD; i++) {
		if (aquad[i].ghost == talvez) {
			printf("%d : ", i);
			for (int j=0; j < QUAD; j++) {
				if (know[i][j]) {
					printf("%d ", j);
				}
			}
			printf("\n");
		}
	}
}

void leapofaith(Agent *agent, int **world) {
	List *paths = iniLst();
	for (int i=0; i < QUAD; i++) {
		if (world[agent->coord][i])
			pshLst(paths,i);
	}

	int rndmov = GetRandomValue(0,paths->size-1);
	int moveto = lstidx(paths,rndmov);
	move(agent,moveto);
	agent->score -= 1;

	clrLst(paths);
}

void move(Agent *agent, int quad) {
	agent->coord = quad;
}

int wasted(Agent *agent, Quad *wquad) {
	int pos = agent->coord;
	if (wquad[pos].ghost) return 1;
	if (wquad[pos].traps) return 1;
	return 0;
}

int winner(Agent *agent, Know *aquad) {
	int pos = agent->coord;
	if (aquad[pos].gold) return 1;
	return 0;
}