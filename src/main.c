#include "includes/main.h"
#include <raylib.h>

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));
	InitWindow(320,240,"World of Wumpus Reloaded");
	InitAudioDevice();

	Music music = LoadMusicStream("resources/metalgear.ogg");
	PlayMusicStream(music);

	Vector2 position;
	Texture2D tex_agent  = LoadTexture("resources/agent.png");
	Texture2D tex_floor  = LoadTexture("resources/floor.png");
	Texture2D tex_trap   = LoadTexture("resources/trap.png");
	Texture2D tex_strap  = LoadTexture("resources/strap.png");
	Texture2D tex_wumpus = LoadTexture("resources/wumpus.png");
	Texture2D tex_chest  = LoadTexture("resources/chest.png");

	Rectangle frameRec = {   0, 0, 32, 32 };
	Rectangle wborder  = {  20,60,128,128 };
	Rectangle aborder  = { 168,60,128,128 };

	Rectangle arooms[ROOM] = { 0 };
	Rectangle wrooms[ROOM] = { 0 };
	for (int i = 0; i < ROOM; i++) {
		arooms[i].x =  20 + 32*(i%WCOL); // separar: '+ px*(i%WCOL)'
		wrooms[i].x = 168 + 32*(i%WCOL); // separar: '+ px*(i/WCOL)'
		arooms[i].y = wrooms[i].y = 60 + 32*(i/WCOL);
		arooms[i].width = wrooms[i].width = 32;
		arooms[i].height = wrooms[i].height = 32;
	}

	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 8; // Number of spritesheet frames shown by second
	SetTargetFPS(60);

	Room *wroom = initRoom();
	int **world = alocArray(ROOM,ROOM);
	iniGraph(world);
	genWorld(world,wroom);
	setVgrau(world,wroom);

	Agent *agent = iniAgent();

	#ifdef DEBUG
	prtGraph(world);
	prtAdjac(world);
	#endif

	// Main game loop
	while (!WindowShouldClose()) {
		// Update variables
		framesCounter++;

		if (framesCounter >= (60/framesSpeed)) {
			framesCounter = 0;
			currentFrame++;
			if (currentFrame > 5)
				currentFrame = 0;
			frameRec.x = (float)currentFrame*(float)tex_agent.width/2;
		}

		// Draw

		BeginDrawing();

			ClearBackground(RAYWHITE);

			UpdateMusicStream(music);

			DrawText("Score 0", 20, 20, 20, DARKGRAY);
			DrawText("Aperte r para resetar", 20, 205, 10, DARKGRAY);
			DrawText("Aperte spacebar para começar", 20, 215, 10, DARKGRAY);

			if (IsKeyPressed(KEY_R))
				rstWorld(world,wroom);

			if (IsKeyPressed(KEY_SPACE))
				rstWorld(world,wroom);

			for (int i = 0; i < ROOM; i++) {
				DrawTexture(tex_floor, arooms[i].x, arooms[i].y, WHITE);
				position.x = arooms[0].x;
				position.y = arooms[0].y;
				DrawTextureRec(tex_agent,frameRec,position,WHITE);
			}

			for (int i = 0; i < ROOM; i++) {
				DrawTexture(tex_floor, wrooms[i].x, wrooms[i].y, WHITE);
				if (wroom[i].gold)
					DrawTexture(tex_chest, wrooms[i].x, wrooms[i].y, WHITE);
				if (wroom[i].whell) {
					position.x = wrooms[i].x;
					position.y = wrooms[i].y;
					DrawTextureRec(tex_strap,frameRec,position,WHITE);
					//DrawTexture(tex_trap, wrooms[i].x, wrooms[i].y, WHITE);
				}
				if (wroom[i].wumpus) {
					position.x = wrooms[i].x;
					position.y = wrooms[i].y;
					DrawTextureRec(tex_wumpus,frameRec,position,WHITE);
				}
			}

			DrawRectangleLinesEx(wborder, 2, DARKGRAY);
			DrawRectangleLinesEx(aborder, 2, DARKGRAY);

		EndDrawing();
	}

	// De-Initialization
	freeArray(ROOM,world);
	free(wroom);
	free(agent);

	UnloadTexture(tex_chest);
	UnloadTexture(tex_wumpus);
	UnloadTexture(tex_trap);
	UnloadTexture(tex_strap);
	UnloadTexture(tex_floor);
	UnloadTexture(tex_agent);
	UnloadMusicStream(music);

	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}