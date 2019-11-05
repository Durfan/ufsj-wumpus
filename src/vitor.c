#include "main.h"

void iniSttes(List *list) {
	for (int i=1; i < WCOL; i++)
		pshLst(list,i);
	for (int i=1; i < WROW; i++) {
		for (int j=0; j < WCOL; j++) {
			if (i % 2)
				pshLst(list,WCOL*i+(WROW -j-1));
			else
				pshLst(list,WCOL*i+j);
		}
	}
}

void CrazyMode(Know *aquad, List *stateList, int *target) {
	*target = bestSmell(aquad);
	pshLst(stateList, *target);
	aquad[*target].traps = nope;
}

void findVTargets(Agent *agent, Know *aquad, List *stateList) {
	List *route = iniLst();
	for (int i = 0; i < QUAD; i++) {
		if (!aquad[i].visit) {
			route = BSF(aquad,agent,i);
			if (!lstnil(route)) pshLst(stateList, i);
			//clrLst(route);
		}
	}
}

int distStates(int x, int y) {
	return (x/WCOL+y/WCOL) + (x%WROW + y%WROW);
}

List *findPosKillGhost(Agent *agent, Know *aquad, int target, List *route) {
	int targetCol = target % WCOL, position, bestDist = 1000, bestPos = -1;
	int targetRow = (int)(target / WCOL);

	for (int i = 0; i < WROW; i++) {
		position = i*WCOL + targetCol;
		if (position != targetCol) {
			route = BSF(aquad, agent, position);
			if (!lstnil(route)) {
				if (distStates(agent->coord, position) < bestDist) {
					bestDist = distStates(agent->coord, position);
					bestPos = position;
				}
			}
			clrLst(route);
		}
	}
	for (int i = 0; i < WCOL; i++) {
		position = i + targetRow*WROW;
		if (position != targetCol) {
			route = BSF(aquad, agent, position);
			if (!lstnil(route)) {
				if (distStates(agent->coord, position) < bestDist) {
					bestDist = distStates(agent->coord, position);
					bestPos = position;
				}
			}
			clrLst(route);
		}
	}
	if (bestPos != -1)route = BSF(aquad, agent, bestPos);
	else route = iniLst();
	return route;
}

int getStateGhost(Know *aquad) {
	for (int i = 0; i < QUAD; i++)
		if (aquad[i].ghost == certeza) return i;
	return -1;
}

int bestMaybGhost(Know *aquad, int *posGhost) {
	List *adjCandidates = iniLst();
	int numAdj, bestCandidate = -1, candidate;
	float prob, bestProb = -1.0;

	for (int i=0; i < QUAD; i++) {
		if (aquad[i].ghost == talvez) {
			addAdjLt(adjCandidates,i);
			numAdj = adjCandidates->size;
			prob = 0.0;

			for (int j=0; j < numAdj; j++) {
				candidate = lstidR(adjCandidates,0);
				if (aquad[candidate].smell == true)
					prob += 1.0;
				else if (!aquad[candidate].visit)
					prob += 0.5;
			}
			if (prob == 0.0)
				prob = 0.00001;
			prob = prob/(float)numAdj;
	
			if (prob > bestProb) {
				bestProb = prob;
				bestCandidate = i;
				*posGhost = i;
			}
		}
	}
	clrLst(adjCandidates);
	return bestCandidate;
}

int ArrowMoviment(Quad *wquad, int coord, int target, int sum, Know *aquad, int t) {
	for (int i=coord; i < target; i++) {
		if (wquad[t].ghost) {
			wquad[t].ghost = false;
			aquad[t].ghost = nope;
			if ((t+4) < QUAD)
				wquad[t+4].smell = aquad[t+4].smell = false;
			if ((t-4) > 0)
				wquad[t-4].smell = aquad[t-4].smell = false;
			if ((t+1) < QUAD)
				wquad[t+1].smell = aquad[t+1].smell = false;
			if ((t-1) > 0)
				wquad[t-1].smell = aquad[t-1].smell = false;
			return 1;
		}
		t += sum;
	}
	return 0;
}

int agentAtack(Agent *agent, int target, Quad *wquad, Know *aquad) {
	int kill = 0;
	if (agent->arrow) {
		int coord = agent->coord;
		if ((target % WCOL) == (coord % WCOL)) { // Mesma linha
			if ((coord - target) < 0) // sentido crescente
				kill = ArrowMoviment(wquad, agent->coord/WCOL, WCOL, 4, aquad, coord);
			else
				kill = ArrowMoviment(wquad, 0, agent->coord/WCOL+1, -4, aquad, coord);
			agent->arrow = false;
		} else if (target / WCOL == agent->coord / WCOL) { // mesma coluna
			if ((coord - target) < 0)
				kill = ArrowMoviment(wquad, agent->coord%WCOL, WCOL, 1, aquad, coord);
			else
				kill = ArrowMoviment(wquad, 0, agent->coord%WCOL+1,-1, aquad, coord);
			agent->arrow = false;
		}
	}
	return kill;
}

List *vitor(Agent *agent, Know *aquad, int **world, List *stateList, List *route, Quad *wquad) {
	int target, moveto;

	if (agent->killerTarget) {
		if (!lstnil(route)) {
			moveto = lstidR(route,0);
			move(agent,moveto);
			agent->score -= 1;

			if (!route->size) {
				if (agentAtack(agent, agent->killerTarget,wquad,aquad))
					agent->grito = true;
				agent->arrow = false;
				agent->killerTarget = 0;
				agent->score -= 1;
			}
		}
	}
	else if (!lstnil(route)) {
		moveto = lstidR(route,0);
		move(agent,moveto);
		agent->score -= 1;

	}
	else if (!lstnil(stateList)) {// explore states
		while (lstnil(route) && !lstnil(stateList)) {
			target = lstidR(stateList,stateList->size-1);
			route = BSF(aquad,agent,target);
		}
		if (!lstnil(route)) {
			moveto = lstidR(route,0);
			move(agent,moveto);
			agent->score -= 1;
		}

	}
	else {
		findVTargets(agent,aquad,stateList); // find valide route;
		if (!lstnil(stateList))
			route = vitor(agent,aquad,world,stateList,route,wquad);
		else { // if not valid path
			if (agent->arrow) { // active killer mode?
				int GhostPos = getStateGhost(aquad); // true ghost position

				if (GhostPos == -1)
					bestMaybGhost(aquad, &GhostPos); // maybe ghost position
				clrLst(route);

				if (GhostPos != -1)
					route = findPosKillGhost(agent, aquad, GhostPos, route);// route to kill ghost

				if (!lstnil(route)) { // active killer mode!
					agent->killerTarget = GhostPos;
					route = vitor(agent,aquad,world,stateList,route,wquad);
				}
				else { // active crazy mode - go to die
					CrazyMode(aquad, stateList, &target);
				}
			}
			else { // active crazy mode - go to die
				CrazyMode(aquad, stateList, &target);
			}
		}
	}
	return route;
}

List *BSF(Know *aquad, Agent *agent, int target) {
	List *route = iniLst();
	int coord = agent->coord;

	if (coord == target)
		return route;

	int candidate, sizeList;
	int visited[QUAD], parents[QUAD];

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
				if (aquad[candidate].wind == true)
					prob += 1.0;
				else if (!aquad[candidate].visit)
					prob += 0.5;
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
