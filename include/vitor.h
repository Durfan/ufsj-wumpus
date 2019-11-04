#ifndef INCLUDE_VITOR_H
#define INCLUDE_VITOR_H

void iniSttes(List *list);
List *omgigoingtodie(Agent *agent, Know *aquad, int **world);
List *vitor(Agent *agent, Know *aquad, int **world, List *stateList, List *route);
List *BSF(Know *aquad, Agent *agent, int target);
void adjNKnow(List *list, Know *aquad, int coord, int *visited, int *parents);
void addVNode(List *list, Know *aquad, int state, int *visited, int *parents, int parent);
void addAdjLt(List *list, int coord);
void genRoute(List *route, int *parents, int init, int target);
int bestSmell(Know *aquad);

//#pragma message __FILE__
#endif // INCLUDE_VITOR_H