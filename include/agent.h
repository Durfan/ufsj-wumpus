#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

Agent *iniAgent(void);

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know);
void chkghost(Agent *agent, Know *aquad, int **know);
void chktraps(Know *aquad, int **know);
int boundary(int v);
void leapofaith(Agent *agent, int **world);
void move(Agent *agent, int quad);

int wasted(Agent *agent, Quad *wquad);
int winner(Agent *agent, Know *aquad);

//#pragma message __FILE__
#endif // INCLUDE_AGENT_H