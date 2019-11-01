#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

Agent *iniAgent(void);
Sensor scanQuad(Agent *agent, Quad *wquad);
void scanPath(Agent *agent, int **world, int **know);
void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know);
void leapofaith(Agent *agent, int **world);
void move(Agent *agent, int quad);
int getpos(Agent *agent);
int wasted(Agent *agent, Quad *wquad);

void showInfos(Sensor sensor);
void prtSensor(Sensor sensor);

//#pragma message __FILE__
#endif // INCLUDE_AGENT_H