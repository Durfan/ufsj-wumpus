#include "includes/main.h"

void manual(Agent *agent);

int main(void) {

	setlocale(LC_ALL,"");
	srand(time(NULL));
	InitWindow(320,240,"World of Wumpus Reloaded");
	InitAudioDevice();

	Music music = LoadMusicStream("resources/snd_mgear.ogg");
	Sound dead  = LoadSound("resources/snd_scream.ogg");
	PlayMusicStream(music);

	Vector2 position;
	Texture2D tex_gold  = LoadTexture("resources/gold.png");
	Texture2D tex_agent = LoadTexture("resources/agent.png");
	Texture2D tex_ghost = LoadTexture("resources/ghost.png");
	Texture2D tex_traps = LoadTexture("resources/traps.png");
	Texture2D tex_trapf = LoadTexture("resources/trapf.png");
	Texture2D tex_trapd = LoadTexture("resources/trapd.png");
	Texture2D tex_floor = LoadTexture("resources/floor.png");

	Rectangle frmRecS = {   0, 0, 32, 32 };
	Rectangle frmRecO = {   0, 0, 32, 32 };
	Rectangle wborder = {  20,60,128,128 };
	Rectangle aborder = { 168,60,128,128 };

	Rectangle aquads[QUAD] = { 0 };
	Rectangle wquads[QUAD] = { 0 };
	for (int i = 0; i < QUAD; i++) {
		aquads[i].x =  20 + 32*(i%WCOL); // separar: '+ px*(i%WCOL)'
		wquads[i].x = 168 + 32*(i%WCOL); // separar: '+ px*(i/WCOL)'
		aquads[i].y = wquads[i].y = 60 + 32*(i/WCOL);
		aquads[i].width = wquads[i].width = 32;
		aquads[i].height = wquads[i].height = 32;
	}

	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 8; // Number of spritesheet frames shown by second
	SetTargetFPS(60);

	Quad *wquad = iniQuad();
	int **world = alocArray(QUAD,QUAD);
	iniGraph(world);
	genWorld(world,wquad);

	Agent *agent = iniAgent();
	Know *aquad = iniKnow();
	Sensor sensor;
	bool lives = true;
	int **know = alocArray(QUAD,QUAD);
	int posagent;

	#ifdef DEBUG
	prtGraph(world);
	prtAdjac(world);
	#endif

	// Main game loop
	while (!WindowShouldClose()) {
		// Update variables
		UpdateMusicStream(music);

		framesCounter++;

		if (framesCounter >= (60/framesSpeed)) {
			framesCounter = 0;
			currentFrame++;
			if (currentFrame > 5)
				currentFrame = 0;
			frmRecS.x = currentFrame * 32;
			frmRecO.x = (currentFrame/2) * 32;
		}

		posagent = getpos(agent);

		if (wasted(agent,wquad)) {
			StopMusicStream(music);
			DrawText("WASTED", 210, 20, 20, RED);
			framesCounter = 0;
			if (lives) {
				PlaySound(dead);
				lives = false;
			}
		}

		sensor = scanQuad(agent,wquad);
		scanPath(agent,world,know);
		ifengine(agent,sensor,aquad,know);

		if (IsKeyPressed(KEY_R)) {
			rstWorld(world,know,agent,wquad,aquad);
			PlayMusicStream(music);
			lives = true;
		}

		if (IsKeyPressed(KEY_SPACE))
			leapofaith(agent,world);

		manual(agent);

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText(FormatText("Score %05d", agent->score), 20, 20, 20, DARKGRAY);
			DrawText("Aperte r para resetar", 20, 205, 10, DARKGRAY);
			DrawText("Aperte [spacebar] para avançar", 20, 215, 10, DARKGRAY);

			showInfos(sensor);

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(tex_floor, aquads[i].x, aquads[i].y, WHITE);
				position.x = aquads[posagent].x;
				position.y = aquads[posagent].y;
				DrawTextureRec(tex_agent,frmRecS,position,WHITE);
				if (aquad[i].visit) {
					position.x = aquads[i].x + 16;
					position.y = aquads[i].y + 16;
					DrawCircleV(position,3,DARKGRAY);
				}
				if (aquad[i].traps == -1)
					DrawTexture(tex_trapd, aquads[i].x, aquads[i].y, WHITE);
				if (aquad[i].traps == 1)
					DrawTexture(tex_trapf, aquads[i].x, aquads[i].y, WHITE);
			}

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(tex_floor, wquads[i].x, wquads[i].y, WHITE);
				position.x = wquads[posagent].x;
				position.y = wquads[posagent].y;
				DrawTextureRec(tex_agent,frmRecS,position,WHITE);
				position.x = wquads[i].x;
				position.y = wquads[i].y;
				if (wquad[i].gold)
					DrawTextureRec(tex_gold,frmRecO,position,WHITE);
				if (wquad[i].traps)
					DrawTextureRec(tex_traps,frmRecO,position,WHITE);
				if (wquad[i].ghost)
					DrawTextureRec(tex_ghost,frmRecS,position,WHITE);
			}

			DrawRectangleLinesEx(wborder, 2, DARKGRAY);
			DrawRectangleLinesEx(aborder, 2, DARKGRAY);

		EndDrawing();
	}

	// De-Initialization
	freeArray(QUAD,world);
	freeArray(QUAD,know);
	free(wquad);
	free(aquad);
	free(agent);

	UnloadTexture(tex_gold);
	UnloadTexture(tex_agent);
	UnloadTexture(tex_ghost);
	UnloadTexture(tex_traps);
	UnloadTexture(tex_trapf);
	UnloadTexture(tex_trapd);
	UnloadTexture(tex_floor);
	UnloadMusicStream(music);
	UnloadSound(dead);

	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}

void manual(Agent *agent) {

	if (IsKeyPressed(KEY_DOWN)) {
		if (agent->coord < (QUAD-WROW)) {
			agent->coord += WROW;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_UP)) {
		if (agent->coord > 0) {
			agent->coord -= WROW;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_RIGHT)) {
		if (agent->coord < QUAD) {
			agent->coord += 1;
			agent->score -= 1;
		}
	} else if (IsKeyPressed(KEY_LEFT)) {
		if (agent->coord > 0) {
			agent->coord -= 1;
			agent->score -= 1;
		}
	}

}