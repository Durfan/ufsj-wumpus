#ifndef INCLUDE_VITOR_H
#define INCLUDE_VITOR_H


int ArrowMoviment(Quad *wquad, int coord, int target, int sum, Know *aquad, int t);
int AtackEffect(Agent *agent, int target, Quad *wquad, Know *aquad);
int choiceInsecureTarget(Know *aquad);
int distStates(int x, int y);
int findPosKillGhost(Agent *agent, Know *aquad, int target);
int getStateGhost(Know *aquad);
int bestMaybGhost(Know *aquad, int *posGhost);
int choiceSafeTarget(Agent *agent, Know *aquad);
int choiceGhostTarget(Know *aquad);
void agentAtack(Agent *agent, Quad *wquad, Know* aquad);
void genRoute(List *route, int *parents, int init, int target);
List *BSF(Know *aquad, Agent *agent, int target);
bool xIsAdj(Agent *agent, int x);
void addVNodeTail(List *list, Know *aquad, int state, int *visited, int *parents, int parent);
void addAdjLt(List *list, int coord);
void adjNKnow(List *list, Know *aquad, int coord, int *visited, int *parents);
int bestTrapForDie(Know *aquad);


bool killerMode(Agent *agent, Know *aquad, Quad *wquad, List *stateList);
bool gotoxMode(Agent *agent, Know *aquad, Quad *wquad, List *stateList);
bool gotodieMode(Agent *agent, Know *aquad, List *stateList);
bool exploreMode(Agent *agent, Know *aquad, List *stateList);
void vitor(Agent *agent, Know *aquad, List *stateList, Quad *wquad);

//#pragma message __FILE__
#endif // INCLUDE_VITOR_H
