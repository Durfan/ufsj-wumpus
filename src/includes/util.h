#ifndef INCLUDE_UTIL_H
#define INCLUDE_UTIL_H

int **alocArray(int n, int m);
void iniGraph(int **array);
int getVgrau(int **array, int v);

void freeArray(int n, int **ptr);

//#pragma message __FILE__
#endif // INCLUDE_UTIL_H