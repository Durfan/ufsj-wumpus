#include "main.h"

int choiceInsecureTarget(Know *aquad) {
	int target = bestTrapForDie(aquad);
	aquad[target].traps = nope;
	return target;
}

int distStates(int x, int y) {
	int dist = (x/WCOL-y/WCOL)*(x/WCOL-y/WCOL) + (x%WROW - y%WROW)*(x%WROW - y%WROW);
    return dist;
}

int findPosKillGhost(Agent *agent, Know *aquad, int target) {
	if (xIsAdj(agent, target)) return agent->coord;

	List *route = iniLst();
	int targetCol = target % WCOL, targetRow = (int)(target / WCOL), position, bestDist = 1000, bestPos = -1;

	for (int i = 0; i < WROW; i++) {
		position = i*WCOL + targetCol;
		if (position != targetCol) {
			BSF(aquad, agent, position, route);
			if (!lstnil(route) && distStates(agent->coord, position) < bestDist) {
				bestDist = distStates(agent->coord, position);
				bestPos = position;
			}
			CLEARLst(route);
		}
	}

	for (int i = 0; i < WCOL; i++) {
		position = i + targetRow*WROW;
		if (position != targetCol) {
			BSF(aquad, agent, position, route);
			if (!lstnil(route) && distStates(agent->coord, position) < bestDist) {
				bestDist = distStates(agent->coord, position);
				bestPos = position;
			}
			CLEARLst(route);
		}
	}
	return bestPos;
}

int getStateGhost(Know *aquad) {
	for (int i = 0; i < QUAD; i++)
		if (aquad[i].ghost == certeza) return i;
	return -1;
}

int bestMaybGhost(Know *aquad) {
	List *adjCandidates = iniLst();
	int numAdj, bestCandidate = -1, candidate;
	float prob, bestProb = -1.0;

	for (int i=0; i < QUAD; i++) {
		if (aquad[i].ghost == talvez) {
			addAdjLt(adjCandidates,i);
			numAdj = adjCandidates->size;
			prob = 0.0;

			for (int j=0; j < numAdj; j++) {
				candidate = popLst(adjCandidates);
				if (aquad[candidate].smell == true)
					prob += 1.0;
				else if (!aquad[candidate].visit)
					prob += 0.5;
			}

			if (prob == 0.0) prob = 0.00001;

			prob = prob/(float)numAdj;
			if (prob > bestProb) {
				bestProb = prob;
				bestCandidate = i;
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

int AtackEffect(Agent *agent, int target, Quad *wquad, Know *aquad) {
	int kill = 0;
	if (agent->arrow) {
		int coord = agent->coord;
		if ((target % WCOL) == (coord % WCOL)) { // Mesma linha
			if ((coord - target) < 0){ // sentido crescente
				printf("\n**** ATACK - S\n");
				kill = ArrowMoviment(wquad, agent->coord/WCOL, WCOL, 4, aquad, coord);
			}else
				printf("\n**** ATACK - N\n");
				kill = ArrowMoviment(wquad, 0, agent->coord/WCOL+1, -4, aquad, coord);
			agent->arrow = false;
		} else if ((target / WCOL) == (coord / WCOL)) { // mesma coluna
			if ((coord - target) < 0){
				printf("\n**** ATACK - L\n");
				kill = ArrowMoviment(wquad, agent->coord%WCOL, WCOL, 1, aquad, coord);
			}else{
				printf("\n**** ATACK - O\n");
				kill = ArrowMoviment(wquad, 0, agent->coord%WCOL+1,-1, aquad, coord);
			}
			agent->arrow = false;
		}
	}
	return kill;
}

int choiceSafeTarget(Agent *agent, Know *aquad){
	List *route = iniLst();
	int bestDist = QUAD*QUAD, bestTarget = - 1;
	for(int i = 1; i < QUAD; i++){
		if(!aquad[i].visit){
			BSF(aquad, agent, i, route);
			if(!lstnil(route) && (distStates(agent->coord, i) < bestDist)){
				bestDist = distStates(agent->coord, i);
				bestTarget = i;
			}
			CLEARLst(route);
		}
	}
	return bestTarget;
}

int choiceGhostTarget(Know *aquad){
	int target = -1;
	target = getStateGhost(aquad);
	if(target < 0) target = bestMaybGhost(aquad);
	return target;
}

void agentAtack(Agent *agent, Quad *wquad, Know* aquad){
	if (AtackEffect(agent,agent->ghost,wquad,aquad))
		agent->grito = true;
	agent->arrow = false;
	agent->ghost = 0;
	agent->score -= 1;
	agent->state = explore;
}

int BSF(Know *aquad, Agent *agent, int target, List *route) {
	if(lstnil(route)){
		CLEARLst(route);
	}

	int coord = agent->coord, candidate;
	int visited[QUAD], parents[QUAD];

	for (int i=0; i < QUAD; i++)
		visited[i] = 0;

	List *searchList = iniLst();
	parents[coord] = coord;
	pshTailLst(searchList, coord);

	while (!lstnil(searchList)) {
 		candidate = popLst(searchList);
		if (candidate == target) {
			genRoute(route,parents,coord,target);
			clrLst(searchList);
			return 1;
		}
		adjNKnow(searchList,aquad,candidate,visited,parents);
	}
	clrLst(searchList);
	return 0;
}

void addVNodeTail(List *list, Know *aquad, int state, int *visited, int *parents, int parent) {
	if ((aquad[state].ghost == 0 && aquad[state].traps == 0) && !visited[state]) {
		pshTailLst(list,state);
		parents[state] = parent;
	}
}

void adjNKnow(List *list, Know *aquad, int coord, int *visited, int *parents) {
	visited[coord] = 1;
	if (coord / WCOL != 0)
		addVNodeTail(list,aquad,coord-WCOL,visited,parents,coord);
	if (coord / WCOL != WCOL-1)
		addVNodeTail(list,aquad,coord+WCOL,visited,parents,coord);
	if (coord % WROW != 0)
		addVNodeTail(list,aquad,coord-1,visited,parents,coord);
	if (coord % WROW != WROW-1)
		addVNodeTail(list,aquad,coord+1,visited,parents,coord);
}

bool xIsAdj(Agent *agent, int x){
	int coord = agent->coord;
	if ((coord + 4 == x ) || (coord - 4 == x ) || (coord + 1 == x ) || (coord - 1 == x ))
		return true;
	return false;
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

int bestTrapForDie(Know *aquad) {
	List *doh = iniLst();
	int numAdj, bestie = -1, candidate;
	float prob, bestProb = -1.0;

	for (int i=0; i < QUAD; i++) {

		if (aquad[i].traps == talvez) {
			addAdjLt(doh,i);
			numAdj = doh->size;
			prob = 0.0;

			for (int j=0; j < numAdj; j++) {
				candidate = popLst(doh);
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

bool killerMode(Agent *agent, Know *aquad, Quad *wquad, List *stateList){
	int target = choiceGhostTarget(aquad);
	if(target != -1){// have path to ghost kill
		agent->state = killer;
		agent->ghost = target;
		target = findPosKillGhost(agent, aquad, target);
		if(target != -1){
			BSF(aquad, agent, target, stateList);
	 	}
		if(!stateList->size){// agent atack
			agentAtack(agent, wquad, aquad);
			BSF(aquad, agent,agent->ghost, stateList);
			agent->state = explore;
		}
		return true;
	}
	return false;
}

bool gotoxMode(Agent *agent, Know *aquad, Quad *wquad, List *stateList){
	if(!lstnil(stateList)){
		int x = popLst(stateList);
		if(x >= 0) move(agent, x);
		if(agent->state == killer && !stateList->size)
			agentAtack(agent, wquad, aquad);
		return true;
	}
	return false;
}

bool gotodieMode(Agent *agent, Know *aquad, List *stateList){
	int target = choiceInsecureTarget(aquad);
	if(lstnil(stateList)){
		if(target != -1){
			BSF(aquad, agent, target, stateList);
		}else
			printf("\nALL MAP WAS EXPLORED\n");
		return true;
	}
	return false;
}

bool exploreMode(Agent *agent, Know *aquad, List *stateList){
	int target = choiceSafeTarget(agent, aquad);
	if(target != -1){//explore mode
		agent->state = explore;
		BSF(aquad, agent, target, stateList);
		return true;
	}else{
		return false;
	}
}

void vitor(Agent *agent, Know *aquad, List *stateList, Quad *wquad){
	bool action = false;
	while(!action){
	 	if(lstnil(stateList)){
			if(!exploreMode(agent, aquad, stateList)){
				if(agent->arrow && agent->state != killer)
					action = killerMode(agent, aquad, wquad, stateList);
				if(agent->state != killer)
					action = gotodieMode(agent, aquad, stateList);
			}
		}else{
			action = gotoxMode(agent, aquad, wquad, stateList);
		}
	}
}
