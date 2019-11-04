#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

Agent *iniAgent(void);

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know);
void chkghost(Agent *agent, Know *aquad, int **know);
void chktraps(Know *aquad, int **know);
int boundary(int v);
List *leapofaith(Agent *agent, Know *aquad ,int **world, List *stateList, List *route);
void move(Agent *agent, int quad);
int getStateGhost(Know *aquad);
List *BSF(Know *aquad, Agent *agent, int target);

int wasted(Agent *agent, Quad *wquad);
int winner(Agent *agent, Know *aquad);

//#pragma message __FILE__
#endif // INCLUDE_AGENT_H
