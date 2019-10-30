#include "includes/main.h"

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));

	Room *wroom = initRoom();
	int **world = alocArray(ROOM,ROOM);
	iniGraph(world);
	genWorld(world,wroom);
	
	prtWorld(wroom);

	prtGraph(world);
	prtAdjac(world);

	freeArray(ROOM,world);
	free(wroom);

	return EXIT_SUCCESS;
}