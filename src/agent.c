#include "includes/main.h"

Agent *iniAgent(void) {
	Agent *agent = malloc(sizeof(Agent));
	if (agent == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}

	agent->arrow = 1;
	agent->coord = 0;
	agent->grito = false;
	agent->limit = false;

	return agent;
} 