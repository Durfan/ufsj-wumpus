#ifndef INCLUDE_SENSOR_H
#define INCLUDE_SENSOR_H

Sensor scanQuad(Agent *agent, Quad *wquad);
int scanCord(Agent *agent);
void scanPath(Agent *agent, int **world, int **know);
int scanLimt(Agent *agent, int **know);
int getVgrau(int **array, int v);

void showInfos(Sensor sensor);
void prtSensor(Sensor sensor);

//#pragma message __FILE__
#endif // INCLUDE_SENSOR_H