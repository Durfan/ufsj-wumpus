#include "main.h"

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

void scanPath(Agent *agent, int **world, int **know, Know *aquad) {
	int coord = agent->coord;
	if (aquad[coord].visit) return;

	for (int i=0; i < QUAD; i++) {
		if (world[coord][i]) {
			know[coord][i] = know[i][coord] = 1;
			aquad[coord].paths++;
		}
	}
}

int scanLimt(Agent *agent, Know *aquad) {
	int coord = agent->coord;
	if (aquad[coord].paths == 4) {
		agent->limit = false;
		return 0;
	}
	agent->limit = true;
	return 1;
}

int scanShout(Agent *agent, Quad *wquad){
	for(int i = 0; i < QUAD; i++){
		if(wquad[i].ghost == true){
		 	agent->grito = false;
			return 0;
		}
	}
	agent->grito = true;
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
	if (sensor.gold)
		DrawText("OURO!!!",100,35,10,ORANGE);
	else if (sensor.smell || sensor.wind)
		DrawText("PERIGO!",100,35,10,RED);
}
