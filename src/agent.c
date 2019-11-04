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

void move(Agent *agent, int quad) {
	agent->coord = quad;
}

int wasted(Agent *agent, Know *aquad, Quad *wquad) {
	int pos = agent->coord;
	if (wquad[pos].ghost) {
		aquad[pos].ghost = 1;
		return 1;
	}
	if (wquad[pos].traps) {
		aquad[pos].traps = 1;
		return 1;
	}
	return 0;
}

int winner(Agent *agent, Know *aquad) {
	int pos = agent->coord;
	if (aquad[pos].gold) return 1;
	return 0;
}