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
	Texture2D texgold  = LoadTexture("resources/gold.png");
	Texture2D texagent = LoadTexture("resources/agent.png");
	Texture2D texghost = LoadTexture("resources/ghost.png");
	Texture2D texghosd = LoadTexture("resources/ghosd.png");
	Texture2D textraps = LoadTexture("resources/traps.png");
	Texture2D textrapf = LoadTexture("resources/trapf.png");
	Texture2D textrapd = LoadTexture("resources/trapd.png");
	Texture2D texewind = LoadTexture("resources/ewind.png");
	Texture2D texesmel = LoadTexture("resources/esmel.png");
	Texture2D texfloor = LoadTexture("resources/floor.png");
	Texture2D texhwter = LoadTexture("resources/hwter.png");
	Texture2D texlvbar = LoadTexture("resources/lvbar.png");

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

		posagent = scanCord(agent);

		if (wasted(agent,wquad)) {
			StopMusicStream(music);
			DrawText("WASTED!",190,35,10,RED);
			framesCounter = 0;
			if (agent->lives) {
				agent->lives = false;
				agent->score -= 1000;
				PlaySound(dead);
			}
		} else if (winner(agent,aquad)) {
			StopMusicStream(music);
			DrawText("WINNER!",190,35,10,ORANGE);
			framesCounter = 0;
			if (agent->lives) {
				agent->lives = false;
				agent->score += 1000;
			}
		}

		sensor = scanQuad(agent,wquad);
		scanPath(agent,world,know);
		scanLimt(agent,know);
		ifengine(agent,sensor,aquad,know);

		if (IsKeyPressed(KEY_R)) {
			rstWorld(world,know,agent,wquad,aquad);
			PlayMusicStream(music);
		}

		if (IsKeyPressed(KEY_SPACE))
			leapofaith(agent,world);

		manual(agent);

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText(FormatText("%05d", agent->score),20,35,20,DARKGRAY);
			DrawTexture(texlvbar,20,15,WHITE);
			DrawTexture(texhwter,75,15,WHITE);
			DrawText(FormatText("x %d", agent->arrow),92,20,10,DARKGRAY);
			DrawText("Aperte r para resetar",20,205,10,DARKGRAY);
			DrawText("Aperte [spacebar] para avançar",20,215,10,DARKGRAY);

			DrawText(FormatText("Q%02d L%02d A%02d G%02d W%02d",
				agent->coord, agent->limit, agent->arrow,
				agent->grito, agent->ghost),
				190,15,10,DARKGRAY);

			DrawText(FormatText("W%02d S%02d G%02d",
				sensor.wind, sensor.smell, sensor.gold),
				190,25,10,DARKGRAY);

			showInfos(sensor);

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(texfloor,aquads[i].x,aquads[i].y,WHITE);
				position.x = aquads[posagent].x;
				position.y = aquads[posagent].y;
				DrawTextureRec(texagent,frmRecS,position,WHITE);
				if (aquad[i].visit) {
					position.x = aquads[i].x + 16;
					position.y = aquads[i].y + 16;
					DrawCircleV(position,3,DARKGRAY);
				}
				position.x = aquads[i].x;
				position.y = aquads[i].y;
				if (aquad[i].gold)
					DrawTextureRec(texgold,frmRecO,position,WHITE);
				if (aquad[i].wind)
					DrawTextureRec(texewind,frmRecO,position,WHITE);
				if (aquad[i].smell)
					DrawTextureRec(texesmel,frmRecO,position,WHITE);
				if (aquad[i].ghost == talvez)
					DrawTexture(texghosd,aquads[i].x,aquads[i].y,WHITE);
				if (aquad[i].ghost == certeza)
					DrawTextureRec(texghost,frmRecS,position,WHITE);
				if (aquad[i].traps == talvez)
					DrawTexture(textrapd,aquads[i].x,aquads[i].y,WHITE);
				if (aquad[i].traps == certeza)
					DrawTexture(textrapf,aquads[i].x,aquads[i].y,WHITE);
			}

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(texfloor,wquads[i].x,wquads[i].y,WHITE);
				position.x = wquads[posagent].x;
				position.y = wquads[posagent].y;
				DrawTextureRec(texagent,frmRecS,position,WHITE);
				position.x = wquads[i].x;
				position.y = wquads[i].y;
				if (wquad[i].gold)
					DrawTextureRec(texgold,frmRecO,position,WHITE);
				if (wquad[i].traps)
					DrawTextureRec(textraps,frmRecO,position,WHITE);
				if (wquad[i].ghost)
					DrawTextureRec(texghost,frmRecS,position,WHITE);
			}

			DrawRectangleLinesEx(wborder,2,DARKGRAY);
			DrawRectangleLinesEx(aborder,2,DARKGRAY);

		EndDrawing();
	}

	// De-Initialization
	freeArray(QUAD,world);
	freeArray(QUAD,know);
	free(wquad);
	free(aquad);
	free(agent);

	UnloadTexture(texgold);
	UnloadTexture(texagent);
	UnloadTexture(texghost);
	UnloadTexture(texghosd);
	UnloadTexture(textraps);
	UnloadTexture(textrapf);
	UnloadTexture(textrapd);
	UnloadTexture(texfloor);
	UnloadTexture(texewind);
	UnloadTexture(texesmel);
	UnloadTexture(texhwter);
	UnloadTexture(texlvbar);
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