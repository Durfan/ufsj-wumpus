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


int choiceInsecureTarget(Know *aquad) {
	int target = bestSmell(aquad);
	aquad[target].traps = nope;
	return target;
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
	int dist = (x/WCOL-y/WCOL)*(x/WCOL-y/WCOL) + (x%WROW - y%WROW)*(x%WROW - y%WROW);
    return dist;
}


int findPosKillGhost(Agent *agent, Know *aquad, int target) {
	int targetCol = target % WCOL, position, bestDist = 1000, bestPos = -1;
	int targetRow = (int)(target / WCOL);
	List *route;
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

	return bestPos;
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
				candidate = lstidR(adjCandidates, 0);
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


int killWithAtack(Agent *agent, int target, Quad *wquad, Know *aquad) {
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
	List *route;
	int bestDist = QUAD*QUAD, bestTarget = - 1;
	for(int i = 1; i < QUAD; i++){
		if(!aquad[i].visit){
			route = BSF(aquad, agent, i);
			if(!lstnil(route)){
				if(distStates(agent->coord, i) < bestDist){
					bestDist = distStates(agent->coord, i);
					bestTarget = i;
				}
			}
			clrLst(route);
		}
	}
	return bestTarget;
}


int choiceGhostTarget(Know *aquad){
	int target = -1;
	target = getStateGhost(aquad);
	if(target < 0) target = bestMaybGhost(aquad, &target);
	return target;
}


void agentCalcRouteMov(Agent *agent, List *stateList, int target, Know *aquad){
	clrLst(stateList);
	stateList = BSF(aquad, agent, target);
	int x = lstidR(stateList, 0);
	if(x >= 0){
		move(agent, x);
		agent->score -=1;
	}
}


void agentAtack(Agent *agent, Quad *wquad, Know* aquad){
	if (killWithAtack(agent,agent->killerTarget,wquad,aquad))
		agent->grito = true;
	agent->arrow = false;
	agent->killerTarget = 0;
	agent->score -= 1;
}


void vitor(Agent *agent, Know *aquad, List *stateList, Quad *wquad){
	int target = -1;
	if(lstnil(stateList)){
		target = choiceSafeTarget(agent, aquad);
		if(target != -1){//explore mode
			agent->explore = true;
			agentCalcRouteMov(agent, stateList, target, aquad);
		}else{// killer or crazy
			if(agent->arrow){
				target = choiceGhostTarget(aquad);
				if(target != -1){// have path to ghost kill
					agent->killerTarget = target;
					agent->killer = true;
					target = findPosKillGhost(agent, aquad, target);
					if(target != -1) agentCalcRouteMov(agent, stateList,target, aquad);
					if(!stateList->size){// agent atack
						agentAtack(agent, wquad, aquad);
						agent->killer = false;
					}
				}else{// path to invalid ghost or fantasy does not exist in knowledge
					target = choiceInsecureTarget(aquad);
					if(target != -1){
						agent->gotodie = true;
						agentCalcRouteMov(agent, stateList, target, aquad);
					}else{
						printf("\nALL MAP WAS EXPLORED\n");
						return;
					}
				}
			}else{
				target = choiceInsecureTarget(aquad);
				if(target != -1){
					agent->gotodie = true;
					agent->explore = false;
					agentCalcRouteMov(agent, stateList, target, aquad);
				}else{
					printf("\nALL MAP WAS EXPLORED\n");
					return;
				}
			}
		}
	}else{// move mode
		int x = lstidR(stateList, 0);
		if(x >= 0){
			move(agent, x);
			agent->score -=1;
		}
		if(agent->killerTarget && !stateList->size){
			agentAtack(agent, wquad, aquad);
			agent->killer = false;
		}
	}
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
//	pshLst(searchList, coord);
	pshTailLst(searchList, coord);
	while (!lstnil(searchList)) {
//		candidate = lstidR(searchList, searchList->size-1);
 		candidate = popLst(searchList);
		if (candidate == target) {
			genRoute(route,parents,coord,target);
			clrLst(searchList);
			printf("\nROTA\n");
			prtLst(route);
			return route;
		}

		adjNKnow(searchList,aquad,candidate,visited,parents);
		sizeList = searchList->size-1;

		for (int i = 0; i < sizeList; i++) {
	//		candidate = lstidR(searchList, searchList->size-1);
			candidate = popLst(searchList);
			if (candidate == target) {
				genRoute(route,parents,coord,target);
				clrLst(searchList);
				printf("\nROTA\n");
				prtLst(route);
				return route;
			}
		}
	}


	if (searchList->size)
		clrLst(searchList);
	return route;
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
				candidate = lstidR(doh, 0);
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
	printf("\n DENTRO KILLER MODE");
	if(target != -1){// have path to ghost kill
		agent->killerTarget = target;
		target = findPosKillGhost(agent, aquad, target);
		if(target != -1){
			clrLst(stateList);
			stateList = BSF(aquad, agent, target);
		// agentCalcRouteMov(agent, stateList,target, aquad);
	 	}
		if(!stateList->size){// agent atack
			agentAtack(agent, wquad, aquad);
			agent->killer = false;
			clrLst(stateList);
			stateList = BSF(aquad, agent,agent->killerTarget);
			return true;
		}else{
			agent->gotox = true;
		}
	}else{
		agent->gotodie = true;
		agent->killer = false;
	}
	return false;
}


bool gotoxMode(Agent *agent, Know *aquad, Quad *wquad, List *stateList){
	if(!lstnil(stateList)){
		int x = popLst(stateList);
		if(x >= 0){
			move(agent, x);
			return true;
		}
		if(agent->killerTarget && !stateList->size){
			agentAtack(agent, wquad, aquad);
			agent->killer = false;
			agent->explore = true;
			return true;
		}
	}else{
		agent->gotox = false;
	}
	return false;
}


bool gotodieMode(Agent *agent, Know *aquad, List *stateList){
	int target = choiceInsecureTarget(aquad);
	//agent->gotodie = false;
	if(!agent->explore){
		if(target != -1){
			clrLst(stateList);
			stateList = BSF(aquad, agent, target);
	//		agentCalcRouteMov(agent, stateList, target, aquad);
			agent->gotox = true;
			agent->explore = true;
			return true;
		}else{
			agent->gotodie = false;
			printf("\nALL MAP WAS EXPLORED\n");
		}
	}else{
		agent->gotodie = false;
	}
	return false;
}

bool exploreMode(Agent *agent, Know *aquad, List *stateList, Quad *wquad){
	int target;
 	if(lstnil(stateList)){
		target = choiceSafeTarget(agent, aquad);
		if(target != -1){//explore mode
			agent->explore = true;
			clrLst(stateList);
			stateList = BSF(aquad, agent, target);
			return true;
		}else{
			if(agent->arrow){
				killerMode(agent, aquad, wquad, stateList);
				agent->killer = true;
			}
			agent->explore = false;
			agent->gotox = false;
			gotodieMode(agent, aquad, stateList);
			return false;
		}
	}else{
		gotoxMode(agent, aquad, wquad, stateList);
	}
	return false;
}
