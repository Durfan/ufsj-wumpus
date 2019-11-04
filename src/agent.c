#include "main.h"

Agent *iniAgent(void) {
	Agent *agent = malloc(sizeof(Agent));
	if (agent == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	agent->coord = START;
	agent->lives = true;
	agent->score = 0;
	agent->ghost = 0;
	agent->arrow = 1;
	agent->grito = false;
	agent->limit = false;

	return agent;
}

void ifengine(Agent *agent, Sensor sensor, Know *aquad, int **know) {
	int coord = agent->coord;
	aquad[coord].visit = true;
	TriBol setinf;
	bool visited;

	if (agent->lives) {
		aquad[coord].traps = nope;
		aquad[coord].ghost = nope;
	}

	if (sensor.gold)
		aquad[coord].gold = true;

	if (sensor.smell) {
		aquad[coord].smell = true;
		if (!agent->ghost) {
			for (int i=0; i < QUAD; i++) {
				setinf  = aquad[i].ghost;
				visited = aquad[i].visit;
				if (know[coord][i] &&
					(setinf == noinf) && !visited)
					aquad[i].ghost = talvez;
			}
		}
	}
	else {
		aquad[coord].smell = false;
		if (!agent->ghost) {
			for (int i=0; i < QUAD; i++)
				if (know[coord][i])
					aquad[i].ghost = nope;
		}
	}

	if (sensor.wind) {
		aquad[coord].wind = true;
		for (int i=0; i < QUAD; i++) {
			setinf  = aquad[i].traps;
			visited = aquad[i].visit;
			if (know[coord][i] &&
				(setinf == noinf) && !visited)
				aquad[i].traps = talvez;
		}
	}
	else {
		aquad[coord].wind = false;
		for (int i=0; i < QUAD; i++) {
			if (know[coord][i])
				aquad[i].traps = nope;
		}
	}

	if (!agent->ghost)
		chkghost(agent,aquad,know);
	chktraps(aquad,know);
}

void chkghost(Agent *agent, Know *aquad, int **know) {
	bool found  = false;
	TriBol ginfo;

	for (int i=0; i < QUAD; i++) {
		found = false;
		ginfo = aquad[i].ghost;
		if ((ginfo == talvez) && (getVgrau(know,i) >= 2)) {
			for (int j=0; j < QUAD; j++) {
				if (know[i][j] && !aquad[j].smell)
					found = false;
				else found = true;
			}
		}
		if (found) {
			agent->ghost = i;
			aquad[i].ghost = certeza;
		}
	}

	if (agent->ghost)
		for (int i=0; i < QUAD; i++) {
			ginfo = aquad[i].ghost;
			if (ginfo == talvez || ginfo == noinf)
				aquad[i].ghost = nope;
	}
}

void chktraps(Know *aquad, int **know) {
	bool found  = false;
	TriBol ginfo;

	for (int i=0; i < QUAD; i++) {
		found = false;
		ginfo = aquad[i].traps;
		if ((ginfo == talvez) && (getVgrau(know,i) >= 3)) {
			for (int j=0; j < QUAD; j++) {
				if (know[i][j] && !aquad[j].wind)
					found = false;
				else found = true;
			}
		}
		if (found)
			aquad[i].traps = certeza;
	}
}

List *leapofaith(Agent *agent, Know *aquad ,int **world, List *stateList, List *route) {
	List *paths;
	int target;
	if(!lstnil(route)){
		printf("\nSEGUINDO ROTA");
		prtLst(route);
		int moveto = lstidxR(route, 0);
		move(agent, moveto);
		agent->score -= 10000;
	}else if(!lstnil(stateList)){
		while(lstnil(route) && !lstnil(stateList)){
			target = lstidxR(stateList, stateList->size-1);
			route = BSF(aquad, agent, target);
		}
		printf("lista de nós candidatos");
		prtLst(route);
		printf("\nTarget %d", target);
		printf(", Estado atual %d , rota:", agent->coord);
		prtLst(route);
		if(!lstnil(route)){
			int moveto = lstidxR(route, 0);
			move(agent, moveto);
			agent->score -= 10000;
			}

	}else{
		if(!agent->arrow){
			//add logo menos

		}else{
			printf("\nMODO");
			target = bestMaybeSmell(aquad);
			pshLst(stateList, target);
			aquad[target].traps = nope;
			route = leapofaith(agent, aquad, world, stateList, 	route);
		}
		/*
		paths = iniLst();
		for (int i=0; i < QUAD; i++) {
			if (world[agent->coord][i])
				pshLst(paths,i);
		}

		int rndmov = GetRandomValue(0,paths->size-1);
		int moveto = lstidx(paths,rndmov);
		move(agent,moveto);
		agent->score -= 1;
		clrLst(paths);
		*/
	}
	return route;

}

void move(Agent *agent, int quad) {
	agent->coord = quad;
}


int chkstates(Know *aquad){
	for(int i = 0; i < QUAD; i++){
		if(((aquad[i].ghost == nope && aquad[i].traps == nope) ||
		 	(aquad[i].ghost == false && aquad[i].traps == false)) && !aquad[i].visit){
			return 1;
		}
	}
	return 0;
}


int getStateGhost(Know *aquad){
	for(int i = 0; i < QUAD; i++)
		if(aquad[i].ghost == certeza) return i;
	return -1;
}


void addValidNode(Know *aquad, int *visited, List *list, int state, int *parents, int parent){
	if ((aquad[state].ghost == 0 && aquad[state].traps == 0) && !visited[state]){
		pshLst(list, state);
	//	visited[state] = 1;
		parents[state] = parent;
	}//else printf("\nNó %d invalido|\n", state);
}

// somente para ambientes quadraticos
void adjNodesKnow(List *list, Know *aquad, int coord, int *visited, int *parents){
//	printf("\n EXPANDINDO NO %d\n", coord);
	visited[coord] = 1;

	if(coord / WCOL != 0){
		addValidNode(aquad, visited, list, coord-WCOL, parents, coord);
	}
	if(coord / WCOL != WCOL-1){
		addValidNode(aquad, visited, list, coord+WCOL, parents, coord);
	}
	if(coord % WROW != 0){
		addValidNode(aquad, visited, list, coord-1, parents, coord);
	}
	if(coord % WROW != WROW-1){
		addValidNode(aquad, visited, list, coord+1, parents, coord);
	}
}


void generateRoute(List *route, int *parents, int init, int target){
	do{
		pshLst(route, target);
		target = parents[target];
	}while(target != init);
}

List *BSF(Know *aquad, Agent *agent, int target){
	int coord = agent->coord, candidate, sizeList;
	if(coord == target) return NULL;
	int visited[QUAD], parents[QUAD], father = coord;
	List *route = iniLst();
	if(coord == target){
	 return route;
 	}
	for(int i = 0; i < QUAD; i++) visited[i] = 0;
	List *searchList = iniLst();
	parents[coord] = coord;

	pshLst(searchList, coord);
	while(!lstnil(searchList)){
		candidate = lstidxR(searchList, searchList->size-1);
		if(candidate == target){
			generateRoute(route, parents, coord, target);
			clrLst(searchList);
			return route;
		}
		adjNodesKnow(searchList, aquad, candidate, visited, parents);
		sizeList = searchList->size-1;
		for(int i = 0; i < sizeList; i++){
			candidate = lstidxR(searchList, sizeList - i);
			if(candidate == target){
				generateRoute(route, parents, coord, target);
				clrLst(searchList);
				return route;
			}
		}
	}
	return route;
}


void addAdjList(List *list, int coord){
	if(coord / WCOL != 0 && (coord / WCOL ) < 3){
		pshLst(list, coord+WCOL);
	}
	if(coord / WCOL != WCOL-1 && (coord / WCOL ) > 0){
		pshLst(list, coord-WCOL);
	}
	if(coord % WROW != 0 && (coord % WROW ) > 0){
		pshLst(list, coord-1);
	}
	if((coord % WROW != WROW-1) && (coord % WROW ) < 3){
		pshLst(list, coord+1);
	}
}

int ajdValidesNodes(List *adjCandidates, int *bestCandidate, Know *aquad){
	int candidate, numAdj;
	addAdjList(adjCandidates, *bestCandidate);
	numAdj = adjCandidates->size;
	for(int j = 0; j < numAdj; j++){
		candidate = lstidx(adjCandidates, 0);
		if(aquad[candidate].visit){
			*bestCandidate = candidate;
		}
	}
	return *bestCandidate;
}

// retorne vertice valido que maximiza a probabilidade de existir
int bestMaybGhostAdj(Know *aquad, int *posGhost){
	List *adjCandidates = iniLst();
	int numAdj, bestCandidate = -1, candidate, bestAdj;
	float prob, bestProb = 1000.0;
	for(int i = 0; i < QUAD; i++){
		if(aquad[i].ghost == talvez){
			addAdjList(adjCandidates, i);
			numAdj = adjCandidates->size;
			prob = 0.0;
			for(int j = 0; j < numAdj; j++){
				candidate = lstidxR(adjCandidates, 0);
				if(aquad[candidate].smell == true ||
					!aquad[candidate].visit){
					prob += 1.0;
				}
			}
			if(prob == 0.0) prob = 0.00001;
			prob = prob/(float)numAdj;
			if(prob < bestProb){
				bestProb = prob;
				bestCandidate = i;
				*posGhost = i;
			}
		}
	}

	clrLst(adjCandidates);
	addAdjList(adjCandidates, bestCandidate);
	numAdj = adjCandidates->size;
	//printf("adjacentes a %d", bestCandidate);
	for(int j = 0; j < numAdj; j++){
		bestAdj = lstidx(adjCandidates, 0);
		if(aquad[bestAdj].visit){
			bestCandidate = bestAdj;
			j = numAdj;
		}
	}
//	clrLst(adjCandidates);
	return bestCandidate;
}

int bestMaybeSmell(Know *aquad){
	List *adjCandidates = iniLst();
	int numAdj, bestCandidate = -1, candidate, newPathflag=0;
	float prob, bestProb = -1.0;

	for(int i = 0; i < QUAD; i++){
		if(aquad[i].traps == talvez){
			addAdjList(adjCandidates, i);
			numAdj = adjCandidates->size;
			prob = 0.0;
			for(int j = 0; j < numAdj; j++){
				candidate = lstidxR(adjCandidates, 0);
				if(aquad[candidate].wind == true ||
					!aquad[candidate].visit){
					prob += 1.0;
				}
			}
			prob = prob/(float)numAdj;
			if(prob > bestProb && aquad[i].ghost != certeza){
				bestProb = prob;
				bestCandidate = i;
			}
		}
	}
	return bestCandidate;
}

void ArrowMoviment(Quad *wquad, int coord, int target, int sum, Know *aquad, Agent *agent, int t){
	for(int i = coord; i < target; i++){
		if(wquad[t].ghost){
			wquad[t].ghost = false;
			aquad[t].ghost = nope;
		}
		t += sum;
	}
	printf("\n");
}

int agentAtackAdj(Agent *agent, int target, Quad *wquad, Know *aquad){
	if(agent->arrow){
		int coord = agent->coord;
		if((target % WCOL) == (coord % WCOL)){// Mesma linha
			// atira flecha em direcao a SetTargetFPS
			if((coord - target) < 0){// sentido crescente
			//	ArrowMoviment(wquad, agent->coord%WCOL, WCOL, 1, aquad, agent);

				ArrowMoviment(wquad, agent->coord/WCOL, WCOL, 4, aquad, agent, coord);
			}else{// sentido decrescente
			//	ArrowMoviment(wquad, 0, (agent->coord%WCOL)+1, 4, aquad, agent);
				ArrowMoviment(wquad, 0, agent->coord/WCOL, 4, aquad, agent, coord);
			}
			agent->arrow = false;
			return 1;
		}else if(target / WCOL == agent->coord / WCOL){// mesma coluna
			if((coord - target) < 0){
			//	ArrowMoviment(wquad, agent->coord/WCOL, WCOL, 4, aquad, agent);

				ArrowMoviment(wquad, agent->coord%WCOL, WCOL, 1, aquad, agent, coord);
			}else{
		//		ArrowMoviment(wquad, 0, (agent->coord/WCOL)+1, 4, aquad, agent);

				ArrowMoviment(wquad, 0, agent->coord%WCOL, 4, aquad, agent, coord);
			}
			agent->arrow = false;
			return 1;
		}
	}
	printf("DEU RUIM");
	return -1;
}

int wasted(Agent *agent, Quad *wquad) {
	int pos = agent->coord;
	if (wquad[pos].ghost) return 1;
	if (wquad[pos].traps) return 1;
	return 0;
}

int winner(Agent *agent, Know *aquad) {
	int pos = agent->coord;
	if (aquad[pos].gold) return 1;
	return 0;
}
