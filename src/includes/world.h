#ifndef INCLUDE_WORLD_H
#define INCLUDE_WORLD_H

Room *initRoom(void);
int excluded(List *list, int key);
void genWorld(int **world, Room *wroom);
void prtwchar(bool cond, char *str);
void prtWorld(Room *room);
void prtGraph(int **array);
void prtAdjac(int **array);

//#pragma message __FILE__
#endif // INCLUDE_WORLD_H