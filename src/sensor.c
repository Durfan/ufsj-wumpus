#include "includes/main.h"

Sensor scanQuad(Agent *agent, Quad *wquad) {
	int coord = agent->coord;
	Sensor sensor;
	sensor.smell = wquad[coord].smell;
	sensor.wind  = wquad[coord].wind;
	sensor.gold  = wquad[coord].gold;

	return sensor;
}

int scanCord(Agent *agent) {
	return agent->coord;
}

void scanPath(Agent *agent, int **world, int **know) {
	int coord = agent->coord;
	for (int i=0; i < QUAD; i++)
		if (world[coord][i])
			know[coord][i] = know[i][coord] = 1;
}

int scanLimt(Agent *agent, int **know) {
	if (getVgrau(know,agent->coord) == 4) {
		agent->limit = false;
		return 0;
	}
	agent->limit = true;
	return 1;
}

int getVgrau(int **array, int v) {
	int grau = 0;
	for (int i=0; i < QUAD; i++)
		if (array[v][i])
			grau++;
	return grau;
}

void showInfos(Sensor sensor) {
	if (sensor.smell)
		DrawText("PERIGO!", 100, 40, 10, RED);
	else if (sensor.wind)
		DrawText("PERIGO!", 100, 40, 10, RED);
	else if (sensor.gold)
		DrawText("OURO!!!", 100, 40, 10, ORANGE);
}

void prtSensor(Sensor sensor) {
	printf("Fedor: %s\n", sensor.smell ? "Sim":"Nao");
	printf("Brisa: %s\n", sensor.wind  ? "Sim":"Nao");
	printf(" Ouro: %s\n", sensor.gold  ? "Sim":"Nao");
}