#ifndef INCLUDE_VITOR_H
#define INCLUDE_VITOR_H

void iniSttes(List *list);
List *omgigoingtodie(Agent *agent, Know *aquad, int **world);
List *BSF(Know *aquad, Agent *agent, int target);
void adjNKnow(List *list, Know *aquad, int coord, int *visited, int *parents);
void addVNode(List *list, Know *aquad, int state, int *visited, int *parents, int parent);
void addAdjLt(List *list, int coord);
void genRoute(List *route, int *parents, int init, int target);
int bestSmell(Know *aquad);
void CrazyMode(Know *aquad, List *stateList, int *target);
void findVTargets(Agent *agent, Know *aquad, List *stateList);
List* findPosKillGhost(Agent *agent, Know *aquad, int target, List *route);
int getStateGhost(Know *aquad);
int bestMaybGhost(Know *aquad, int *posGhost);
int ArrowMoviment(Quad *wquad, int coord, int target, int sum, Know *aquad, int t);
int killWithAtack(Agent *agent, int target, Quad *wquad, Know *aquad);
int distStates(int x, int y);
void vitor(Agent *agent, Know *aquad, List *stateList, Quad *wquad);
//#pragma message __FILE__
#endif // INCLUDE_VITOR_H

