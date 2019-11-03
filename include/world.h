#ifndef INCLUDE_WORLD_H
#define INCLUDE_WORLD_H

Quad *iniQuad(void);
Know *iniKnow(void);
int excluded(List *list, int key);
void genWorld(int **world, Quad *wquad);
void prtwchar(bool cond, char *str);
void prtWorld(Quad *quad);
void prtGraph(int **array);
void prtAdjac(int **array);

void rstWorld(int **world, int **know,
	Agent *agent, Quad *wquad, Know *aquad);

//#pragma message __FILE__
#endif // INCLUDE_WORLD_H