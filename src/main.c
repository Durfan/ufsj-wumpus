#include "includes/main.h"
#include "includes/raylib.h"

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));
	const int screenWidth = 320;
	const int screenHeight = 240;
	InitWindow(screenWidth, screenHeight, "World of Wumpus Reloaded");
	InitAudioDevice();

	Music music = LoadMusicStream("resources/metalgear.ogg");
	PlayMusicStream(music);

	Vector2 position;
	Texture2D agent  = LoadTexture("resources/agent.png");
	Texture2D floor  = LoadTexture("resources/floor.png");
	Texture2D trap   = LoadTexture("resources/trap.png");
	Texture2D strap  = LoadTexture("resources/strap.png");
	Texture2D wumpus = LoadTexture("resources/wumpus.png");
	Texture2D chest  = LoadTexture("resources/chest.png");

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

	#ifdef DEBUG
	prtWorld(wroom);
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
			frameRec.x = (float)currentFrame*(float)agent.width/2;
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

			for (int i = 0; i < ROOM; i++) {
				DrawTexture(floor, arooms[i].x, arooms[i].y, WHITE);
				position.x = arooms[0].x;
				position.y = arooms[0].y;
				DrawTextureRec(agent,frameRec,position,WHITE);
			}

			for (int i = 0; i < ROOM; i++) {
				DrawTexture(floor, wrooms[i].x, wrooms[i].y, WHITE);
				if (wroom[i].gold)
					DrawTexture(chest, wrooms[i].x, wrooms[i].y, WHITE);
				if (wroom[i].whell) {
					position.x = wrooms[i].x;
					position.y = wrooms[i].y;
					DrawTextureRec(strap,frameRec,position,WHITE);
					//DrawTexture(trap, wrooms[i].x, wrooms[i].y, WHITE);
				}
				if (wroom[i].wumpus) {
					position.x = wrooms[i].x;
					position.y = wrooms[i].y;
					DrawTextureRec(wumpus,frameRec,position,WHITE);
				}
			}

			DrawRectangleLinesEx(wborder, 2, DARKGRAY);
			DrawRectangleLinesEx(aborder, 2, DARKGRAY);

		EndDrawing();
	}

	// De-Initialization
	freeArray(ROOM,world);
	free(wroom);
	UnloadTexture(chest);
	UnloadTexture(wumpus);
	UnloadTexture(trap);
	UnloadTexture(strap);
	UnloadTexture(floor);
	UnloadTexture(agent);
	UnloadMusicStream(music);
	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}