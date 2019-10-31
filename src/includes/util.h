#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

int **alocArray(int n, int m);
void iniGraph(int **array);
void setVgrau(int **array, Room *room);

void freeArray(int n, int **ptr);

//#pragma message __FILE__
#endif // INCLUDE_UTIL_H