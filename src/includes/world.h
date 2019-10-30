#ifndef INCLUDE_WORLD_H
#define INCLUDE_WORLD_H

Room *initRoom(void);
void genWorld(int **world, Room *wroom);
void prtWorld(Room *room);
void prtGraph(int **array);
void prtAdjac(int **array);

//#pragma message __FILE__
#endif // INCLUDE_WORLD_H