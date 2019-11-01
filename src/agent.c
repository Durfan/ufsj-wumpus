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

Sensor scanQuad(Agent *agent, Quad *wquad) {
	int coord = agent->coord;
	Sensor sensor;
	sensor.smell = wquad[coord].smell;
	sensor.wind  = wquad[coord].wind;
	sensor.gold  = wquad[coord].gold;

	return sensor;
}

void scanPath(Agent *agent, int **world, int **know) {
	int coord = agent->coord;
	for (int i=0; i < QUAD; i++)
		if (world[coord][i])
			know[coord][i] = know[i][coord] = 1;
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

void showInfos(Sensor sensor) {
	if (sensor.smell)
		DrawText("PERIGO!", 20, 40, 10, RED);
	else if (sensor.wind)
		DrawText("PERIGO!", 20, 40, 10, RED);
	else if (sensor.gold)
		DrawText("OURO!!!", 20, 40, 10, ORANGE);
}

void prtSensor(Sensor sensor) {
	printf("Fedor: %s\n", sensor.smell ? "Sim":"Nao");
	printf("Brisa: %s\n", sensor.wind  ? "Sim":"Nao");
	printf(" Ouro: %s\n", sensor.gold  ? "Sim":"Nao");
}