#include "main.h"

Texture2D *texassets(void) {
	Texture2D *texture = malloc(TEXTR * sizeof(Texture2D));
	if (texture == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	texture[0]  = LoadTexture("resources/gold.png");
	texture[1]  = LoadTexture("resources/agent.png");
	texture[2]  = LoadTexture("resources/ghost.png");
	texture[3]  = LoadTexture("resources/ghosd.png");
	texture[4]  = LoadTexture("resources/traps.png");
	texture[5]  = LoadTexture("resources/trapf.png");
	texture[6]  = LoadTexture("resources/trapd.png");
	texture[7]  = LoadTexture("resources/ewind.png");
	texture[8]  = LoadTexture("resources/esmel.png");
	texture[9]  = LoadTexture("resources/floor.png");
	texture[10] = LoadTexture("resources/hwter.png");
	texture[11] = LoadTexture("resources/lvbar.png");
	texture[12] = LoadTexture("resources/ekill.png");
	return texture;
}

Rectangle *drawArray(int x) {
	Rectangle *array = malloc(QUAD * sizeof(Rectangle));
	if (array == NULL) {
		perror(PROGRAM);
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < QUAD; i++) {
		array[i].x =  x + 32*(i%WCOL); // separar: '+ px*(i%WCOL)'
		array[i].y = 60 + 32*(i/WCOL);
		array[i].width = 32;
		array[i].height = 32;
	}
	return array;
}

void clrTxtres(Texture2D *texture) {
	for (int i=0; i < TEXTR; i++)
		UnloadTexture(texture[i]);
	free(texture);
}

void manual(Agent *agent) {
	int coord = agent->coord;

	if (IsKeyPressed(KEY_DOWN)) {
		if (coord < (QUAD-WROW)) {
			agent->coord += WROW;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_UP)) {
		if (coord > 0) {
			agent->coord -= WROW;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_RIGHT)) {
		if (coord < (((coord/WROW)+1) * WCOL)-1) {
			agent->coord += 1;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_LEFT)) {
		if (coord > ((coord/WROW) * WCOL)) {
			agent->coord -= 1;
			agent->score -= 1;
		}
	}

}