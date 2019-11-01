#include "includes/main.h"

Agent *iniAgent(void) {
	Agent *agent = malloc(sizeof(Agent));
	if (agent == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	agent->coord = 0;
	agent->score = 0;
	agent->arrow = 1;
	agent->grito = false;
	agent->limit = false;

	return agent;
}

Sensor scanQuad(Agent *agent, Room *wroom) {
	int coord = agent->coord;
	Sensor sensor;
	sensor.smell = wroom[coord].smell;
	sensor.wind  = wroom[coord].wind;
	sensor.gold  = wroom[coord].gold;
	if (wroom[coord].grau != 4)
		sensor.lmit = true;
	else 
		sensor.lmit = false;

	return sensor;
}

void scanPath(Agent *agent, int **world, int **bknow) {
	int coord = agent->coord;
	for (int i=0; i < ROOM; i++)
		if (world[coord][i])
			bknow[coord][i] = bknow[i][coord] = 1;
}

void ifengine(Agent *agent, Sensor sensor, Bknow *aroom, int **bknow) {
	int coord = agent->coord;
	bool visit,traps,mark;
	aroom[coord].visited = true;

	if (sensor.smell) {
		aroom[coord].smell = true;
		for (int i=0; i < ROOM; i++)
			if (bknow[coord][i] && !aroom[i].visited)
				aroom[i].ghost = 1;
	}

	if (sensor.wind) {
		aroom[coord].wind = true;
		for (int i=0; i < ROOM; i++)
			if (bknow[coord][i] && !aroom[i].visited)
				aroom[i].traps = -1;
	}
	else {
		for (int i=0; i < ROOM; i++)
			if (bknow[coord][i] && (aroom[i].traps == -1))
				aroom[i].traps = 0;	
	}

}

void leapofaith(Agent *agent, int **world) {
	List *paths = iniLst();
	for (int i=0; i < ROOM; i++) {
		if (world[agent->coord][i])
			pshLst(paths,i);
	}

	int rndmov = GetRandomValue(0,paths->size-1);
	int moveto = lstidx(paths,rndmov);
	move(agent,moveto);
	agent->score -= 1;

	clrLst(paths);
}

void move(Agent *agent, int room) {
	agent->coord = room;
}

int getpos(Agent *agent) {
	return agent->coord;
}

int wasted(Agent *agent, Room *wroom) {
	int pos = agent->coord;
	if (wroom[pos].ghost) return 1;
	if (wroom[pos].traps) return 1;
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
	printf("Limit: %s\n", sensor.lmit  ? "Sim":"Nao");
}