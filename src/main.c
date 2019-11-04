#include "main.h"


int main(void) {

	setlocale(LC_ALL,"");
	//SetConfigFlags(FLAG_SHOW_LOGO);

	InitWindow(320,240,"World of Wumpus Reloaded");
	InitAudioDevice();

	Image icon = LoadImage("resources/icon.png");
	SetWindowIcon(icon);

	Music music = LoadMusicStream("resources/snd_mgear.ogg");
	Sound dead  = LoadSound("resources/snd_scream.ogg");
	PlayMusicStream(music);

	Vector2 position;
	Texture2D *texture = texassets();

	Rectangle frmRecS = {   0, 0, 32, 32 };
	Rectangle frmRecO = {   0, 0, 32, 32 };
	//Rectangle frmRecK = {   0, 0, 48, 48 };
	Rectangle wborder = {  20,60,128,128 };
	Rectangle aborder = { 168,60,128,128 };

	Rectangle *aquads = drawArray(20);
	Rectangle *wquads = drawArray(168);

	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 8; // Number of spritesheet frames shown by second
	SetTargetFPS(60);

	Sensor sensor;
	Agent *agent = iniAgent();
	Quad *wquad = iniQuad();
	Know *aquad = iniKnow();
	int **world = alocArray(QUAD,QUAD);
	int **know  = alocArray(QUAD,QUAD);
	iniGraph(world);
	genWorld(world,wquad);

	int posagent;
	float time;

	#ifdef DEBUG
	prtGraph(world);
	prtAdjac(world);
	#endif

	int target;
	List *stateList = iniLst();
	List *route = iniLst();
	for(int i = 1; i < QUAD; i++) pshLst(stateList, i);


	prtLst(stateList);
	// Main game loop
	while (!WindowShouldClose()) {
		// Update variables
		UpdateMusicStream(music);

		if (agent->lives)
			time += GetFrameTime();

		framesCounter++;

		if (framesCounter >= (60/framesSpeed)) {
			framesCounter = 0;
			currentFrame++;
			if (currentFrame > 5)
				currentFrame = 0;
			frmRecS.x = currentFrame * 32;
			//frmRecK.x = currentFrame * 48;
			frmRecO.x = (currentFrame/2) * 32;
		}

		posagent = scanCord(agent);

		if (wasted(agent,wquad)) {
			StopMusicStream(music);
			DrawText("WASTED!",100,45,10,RED);
			framesCounter = 0;
			if (agent->lives) {
				agent->lives = false;
				agent->score -= 1000;
				PlaySound(dead);
			}
		} else if (winner(agent,aquad)) {
			StopMusicStream(music);
			DrawText("WINNER!",100,45,10,ORANGE);
			framesCounter = 0;
			if (agent->lives) {
				agent->lives = false;
				agent->score += 1000;
			}
		}

		sensor = scanQuad(agent,wquad);
		scanPath(agent,world,know,aquad);
		scanLimt(agent,aquad);
		ifengine(agent,sensor,aquad,know);


		if (IsKeyPressed(KEY_R)) {
			time = 0;
			rstWorld(world,know,agent,wquad,aquad);
			PlayMusicStream(music);
		}

		if (IsKeyPressed(KEY_SPACE)){
			route = leapofaith(agent, aquad, world, stateList, route);
		}
		manual(agent);

		// Draw
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawText(FormatText("%05d", agent->score),20,35,20,DARKGRAY);
			DrawTexture(texture[11],20,15,WHITE);
			DrawTexture(texture[10],75,15,WHITE);
			DrawText(FormatText("x %d", agent->arrow),92,20,10,DARKGRAY);
			DrawText("Aperte r para resetar",20,205,10,DARKGRAY);
			DrawText("Aperte [spacebar] para avançar",20,215,10,DARKGRAY);

			DrawText(FormatText("Q%02d L%02d A%02d G%02d W%02d",
				agent->coord, agent->limit, agent->arrow,
				agent->grito, agent->ghost),
				180,15,10,DARKGRAY);
			DrawText(FormatText("W%02d S%02d G%02d P%02d",
				sensor.wind, sensor.smell, sensor.gold, aquad[posagent].paths),
				180,25,10,DARKGRAY);
			DrawText(FormatText("T %02.02f",time),180,35,10,DARKGRAY);

			showInfos(sensor);

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(texture[9],aquads[i].x,aquads[i].y,WHITE);
				position.x = aquads[posagent].x;
				position.y = aquads[posagent].y;
				DrawTextureRec(texture[1],frmRecS,position,WHITE);
				if (aquad[i].visit) {
					position.x = aquads[i].x + 16;
					position.y = aquads[i].y + 16;
					DrawCircleV(position,3,DARKGRAY);
				}
				position.x = aquads[i].x;
				position.y = aquads[i].y;
				if (aquad[i].gold)
					DrawTextureRec(texture[0],frmRecO,position,WHITE);
				if (aquad[i].wind)
					DrawTextureRec(texture[7],frmRecO,position,WHITE);
				if (aquad[i].smell)
					DrawTextureRec(texture[8],frmRecO,position,WHITE);
				if (aquad[i].ghost == talvez)
					DrawTexture(texture[3],aquads[i].x,aquads[i].y,WHITE);
				if (aquad[i].ghost == certeza)
					DrawTextureRec(texture[2],frmRecS,position,WHITE);
				if (aquad[i].traps == talvez)
					DrawTexture(texture[6],aquads[i].x,aquads[i].y,WHITE);
				if (aquad[i].traps == certeza)
					DrawTexture(texture[5],aquads[i].x,aquads[i].y,WHITE);
			}

			for (int i = 0; i < QUAD; i++) {
				DrawTexture(texture[9],wquads[i].x,wquads[i].y,WHITE);
				position.x = wquads[posagent].x;
				position.y = wquads[posagent].y;
				DrawTextureRec(texture[1],frmRecS,position,WHITE);
				position.x = wquads[i].x;
				position.y = wquads[i].y;
				if (wquad[i].gold)
					DrawTextureRec(texture[0],frmRecO,position,WHITE);
				if (wquad[i].traps)
					DrawTextureRec(texture[4],frmRecO,position,WHITE);
				if (wquad[i].ghost)
					DrawTextureRec(texture[2],frmRecS,position,WHITE);
			}

			DrawRectangleLinesEx(wborder,2,DARKGRAY);
			DrawRectangleLinesEx(aborder,2,DARKGRAY);

		EndDrawing();

	}

	// De-Initialization
	freeArray(QUAD,world);
	freeArray(QUAD,know);
	free(wquad);
	free(wquads);
	free(aquad);
	free(aquads);
	free(agent);

	clrTxtres(texture);
	UnloadSound(dead);
	CloseAudioDevice();
	CloseWindow();

	return EXIT_SUCCESS;
}
