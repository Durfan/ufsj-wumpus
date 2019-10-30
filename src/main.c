#include "includes/main.h"

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));

	iniStructs();
	genWorld();

	prtWorld();
	//prtGraph();
	//prtAdjac();

	return EXIT_SUCCESS;
}