#include "includes/main.h"

Sensor scanRoom(Agent *agent, Room *wroom) {
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

void prtSensor(Sensor sensor) {
	printf("Fedor: %s\n", sensor.fedor ? "Sim":"Nao");
	printf("Brisa: %s\n", sensor.brisa ? "Sim":"Nao");
	printf(" Ouro: %s\n", sensor.gold  ? "Sim":"Nao");
	printf("Limit: %s\n", sensor.limit ? "Sim":"Nao");
}