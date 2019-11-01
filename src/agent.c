#include "includes/main.h"

Agent *iniAgent(void) {
	Agent *agent = malloc(sizeof(Agent));
	if (agent == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	agent->coord = START;
	agent->score = 0;
	agent->arrow = 1;
	agent->grito = false;
	agent->limit = false;

	return agent;
}

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know) {
	int coord = agent->coord;
	aquad[coord].visit = true;

	if (sensor.smell) {
		aquad[coord].smell = true;
		for (int i=0; i < QUAD; i++)
			if (know[coord][i] && !aquad[i].visit)
				aquad[i].ghost = 1;
	}

	if (sensor.wind) {
		aquad[coord].wind = true;
		for (int i=0; i < QUAD; i++)
			if (know[coord][i] && !aquad[i].visit)
				aquad[i].traps = -1;
	}
	else {
		for (int i=0; i < QUAD; i++)
			if (know[coord][i] && (aquad[i].traps == -1))
				aquad[i].traps = 0;	
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

int getpos(Agent *agent) {
	return agent->coord;
}

int wasted(Agent *agent, Quad *wquad) {
	int pos = agent->coord;
	if (wquad[pos].ghost) return 1;
	if (wquad[pos].traps) return 1;
	return 0;
}