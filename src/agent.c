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
	sensor.fedor = wroom[coord].fedor;
	sensor.brisa = wroom[coord].brisa;
	sensor.gold  = wroom[coord].gold;
	if (wroom[coord].grau != 4)
		sensor.limit = true;
	else 
		sensor.limit = false;

	return sensor;
}

void scanPath(Agent *agent, int **world, int **bknow) {
	int coord = agent->coord;
	for (int i=0; i < ROOM; i++)
		if (world[coord][i])
			bknow[coord][i] = bknow[i][coord] = 1;
}

void ifengine(Agent *agent, Sensor sensor, int **bknow) {
	int coord = agent->coord;
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
	if (wroom[pos].wumpus) return 1;
	if (wroom[pos].whell)  return 1;
	return 0;
}

void showInfos(Sensor sensor) {
	if (sensor.fedor)
		DrawText("PERIGO!", 20, 40, 10, RED);
	else if (sensor.brisa)
		DrawText("PERIGO!", 20, 40, 10, RED);
	else if (sensor.gold)
		DrawText("OURO!!!", 20, 40, 10, ORANGE);
}

void prtSensor(Sensor sensor) {
	printf("Fedor: %s\n", sensor.fedor ? "Sim":"Nao");
	printf("Brisa: %s\n", sensor.brisa ? "Sim":"Nao");
	printf(" Ouro: %s\n", sensor.gold  ? "Sim":"Nao");
	printf("Limit: %s\n", sensor.limit ? "Sim":"Nao");
}