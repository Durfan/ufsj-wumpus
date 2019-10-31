#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

Agent *iniAgent(void);
Sensor scanQuad(Agent *agent, Room *wroom);
void scanPath(Agent *agent, int **world, int **bknow);
void ifengine(Agent *agent, Sensor sensor, int **bknow);
void leapofaith(Agent *agent, int **world);
void move(Agent *agent, int room);
int getpos(Agent *agent);
int wasted(Agent *agent, Room *wroom);

void showInfos(Sensor sensor);
void prtSensor(Sensor sensor);

//#pragma message __FILE__
#endif // INCLUDE_AGENT_H