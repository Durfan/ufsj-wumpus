#ifndef INCLUDE_AGENT_H
#define INCLUDE_AGENT_H

Agent *iniAgent(void);

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know);
void chkghost(Agent *agent, Know *aquad, int **know);
void chktraps(Know *aquad, int **know);
void move(Agent *agent, int quad);

int wasted(Agent *agent, Know *aquad, Quad *wquad);
int winner(Agent *agent, Know *aquad);

//#pragma message __FILE__
#endif // INCLUDE_AGENT_H