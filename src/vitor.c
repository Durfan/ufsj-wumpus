#include "main.h"

void iniSttes(List *list) {
	for (int i=1; i < QUAD; i++)
		pshLst(list,i);
}

List *vitor(Agent *agent, Know *aquad, int **world, List *stateList, List *route) {
	int target;

	if (!lstnil(route)) {
		int moveto = lstidR(route,0);
		move(agent,moveto);
		agent->score -= 1;
	}
	else if (!lstnil(stateList)) {
		while (lstnil(route) && !lstnil(stateList)) {
			target = lstidR(stateList,stateList->size-1);
			route = BSF(aquad,agent,target);
		}
		if (!lstnil(route)) {
			int moveto = lstidR(route,0);
			move(agent,moveto);
			agent->score -= 1;
		}
	}
	else {
		if (!agent->arrow) {
			//add logo menos
		}
		else {
			target = bestSmell(aquad);
			pshLst(stateList, target);
			aquad[target].traps = nope;
			route = vitor(agent,aquad,world,stateList,route);
		}
	}

	return route;
}

List *BSF(Know *aquad, Agent *agent, int target) {
	int coord = agent->coord;
	if (coord == target)
		return NULL;

	int candidate,sizeList;
	int visited[QUAD], parents[QUAD];
	List *route = iniLst();

	if (coord == target)
		return route;

	for (int i=0; i < QUAD; i++)
		visited[i] = 0;

	List *searchList = iniLst();
	parents[coord] = coord;
	pshLst(searchList, coord);

	while (!lstnil(searchList)) {
		candidate = lstidR(searchList,searchList->size-1);

		if (candidate == target) {
			genRoute(route,parents,coord,target);
			clrLst(searchList);
			return route;
		}

		adjNKnow(searchList,aquad,candidate,visited,parents);
		sizeList = searchList->size-1;

		for (int i = 0; i < sizeList; i++) {
			candidate = lstidR(searchList, sizeList - i);
			if (candidate == target) {
				genRoute(route,parents,coord,target);
				clrLst(searchList);
				return route;
			}
		}
	}

	if (searchList)
		clrLst(searchList);

	return route;
}

void adjNKnow(List *list, Know *aquad, int coord, int *visited, int *parents) {
	visited[coord] = 1;
	if (coord / WCOL != 0)
		addVNode(list,aquad,coord-WCOL,visited,parents,coord);
	if (coord / WCOL != WCOL-1)
		addVNode(list,aquad,coord+WCOL,visited,parents,coord);
	if (coord % WROW != 0)
		addVNode(list,aquad,coord-1,visited,parents,coord);
	if (coord % WROW != WROW-1)
		addVNode(list,aquad,coord+1,visited,parents,coord);
}

void addVNode(List *list, Know *aquad, int state, int *visited, int *parents, int parent) {
	if ((aquad[state].ghost == 0 && aquad[state].traps == 0) && !visited[state]) {
		pshLst(list,state);
		parents[state] = parent;
	}
}

void addAdjLt(List *list, int coord) {
	if (coord / WCOL != 0 && (coord / WCOL ) < 3)
		pshLst(list,coord+WCOL);
	if (coord % WROW != 0 && (coord % WROW ) > 0)
		pshLst(list,coord-1);
	if ((coord / WCOL != WCOL-1) && (coord / WCOL ) > 0)
		pshLst(list,coord-WCOL);
	if ((coord % WROW != WROW-1) && (coord % WROW ) < 3)
		pshLst(list,coord+1);
}

void genRoute(List *route, int *parents, int init, int target) {
	do {
		pshLst(route, target);
		target = parents[target];
	} while (target != init);
}

int bestSmell(Know *aquad) {
	List *doh = iniLst();
	int numAdj, bestie = -1, candidate;
	float prob, bestProb = -1.0;

	for (int i=0; i < QUAD; i++) {

		if (aquad[i].traps == talvez) {
			addAdjLt(doh,i);
			numAdj = doh->size;
			prob = 0.0;

			for (int j=0; j < numAdj; j++) {
				candidate = lstidR(doh,0);
				if (aquad[candidate].wind == true ||
					!aquad[candidate].visit) {
					prob += 1.0;
				}
			}
			prob = prob/(float)numAdj;
			if (prob > bestProb && aquad[i].ghost != certeza) {
				bestProb = prob;
				bestie = i;
			}
		}
	}
	clrLst(doh);
	return bestie;
}