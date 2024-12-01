#include "game.h"
#include <iostream>

const Color darkGrey{26, 31, 40, 255};
const Color darkRed{ 139, 0, 0, 255 };

Game::Game()
{
	platform.push_back({100, 200,80,10});
	platform.push_back({200, 300,120,10});
	platform.push_back({150, 500,200,10});
	ball.position = {140,180,20,20};
	ball.falling = false;
	easyRectangle = {410, 320, 10, 10};
	mediumRectangle = {410, 350, 10, 10};
	hardRectangle = {410, 380, 10, 10};
	extremeRectangle = {410, 410, 10, 10};
	level = EASY;
}

Game::~Game()
{

}

void Game::setupAudio()
{
	InitAudioDevice(); // Initialize audio device

	if(IsAudioDeviceReady())
	{
		bgMusic = LoadMusicStream("sounds/music.mp3");

		landingSound = LoadSound("sounds/landing.mp3");

		failSound = LoadSound("sounds/fail.mp3");
	}
}

void Game::checkBallFall()
{
	bool falling = true;
	Rectangle newPosition = ball.position;
	newPosition.y += 10;
	for(auto platfm : platform)
	{
		if(CheckCollisionRecs(newPosition, platfm)){
			falling = false;
			ball.position.y = platfm.y-20;
			if(updateScore == true)
			{
				score++;
				PlaySound(landingSound);
				updateScore = false;
			}
			break;
		}
	}
	if(falling == false)
	{
		ball.falling = false;
	}
	else
	{
		updateScore = true;
		ball.falling = true;
	}
}

void Game::updatePlatformAndBall()
{
	checkBallFall();

	if(ball.falling == true){
		if(ball.position.y+10 > WINDOW_HEIGHT - 30){
			gameOver = true;
			PlaySound(failSound);
		}else{
			ball.position.y += 10;
		}
	}
	else{
		if(ball.position.y-5 < 10){
			gameOver = true;
			PlaySound(failSound);
		}else{
			ball.position.y -= 5;
		}

	}
	uint32_t index = platform.size()-1;

	for(int i = 0; i < platform.size(); i++)
	{
		platform[i].y -= 5;
	}
	if(platform[0].y < 10)
	{
		platform.erase(platform.begin());
	}
	if((WINDOW_HEIGHT - platform[index].y) > platformDistance)
	{
		Rectangle newPlatform;
		newPlatform.x = GetRandomValue(10,300);
		newPlatform.y = WINDOW_HEIGHT - 20;
		newPlatform.width = GetRandomValue(40, 340- newPlatform.x);
		newPlatform.height = 10;
		platform.push_back(newPlatform);
		platformCreated = true;
	}
}

void Game::run()
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Falling Ball Game");
	SetTargetFPS(60);

	Texture2D background = LoadTexture("assets/skybackground.png");

	Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height};
	Rectangle destRec = {LUD_BORDER,LUD_BORDER,(WINDOW_WIDTH-LUD_BORDER-RIGHT_BORDER),(WINDOW_HEIGHT - 2 * LUD_BORDER)};

	Texture2D brickwall = LoadTexture("assets/brickwall.png");
	Rectangle bricksrcRec = { 0.0f, 10.0f, (float)brickwall.width, (float)20};

	Texture2D balltexture = LoadTexture("assets/ball.png");
	Rectangle ballRec = { 0.0f, 10.0f, (float)balltexture.width, (float)balltexture.height};

	Vector2 origin = { 0.0f, 0.0f };

	float rotation = 0.0f;
	platformCreated = true;
	gameOver = false;
	PlayMusicStream(bgMusic);
	float timer = 1.0f / level;
	score  = 0;

	while(!WindowShouldClose())
	{
		UpdateMusicStream(bgMusic); // Update audio buffer
		timer -= GetFrameTime();  // Subtract frame time from timer

		if(IsKeyDown(KEY_RIGHT)) 
		{
			if((ball.position.x + 5) > (WINDOW_WIDTH - RIGHT_BORDER - 20)){
				ball.position.x = (WINDOW_WIDTH - RIGHT_BORDER - 20);
			}
			else{
				ball.position.x +=5;
			}
		}
		if(IsKeyDown(KEY_LEFT))
		{
			if((ball.position.x - 5) < 10){
				ball.position.x = 10;
			}
			else{
				ball.position.x -= 5;
			}
		} 

		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();
			if(CheckCollisionPointRec(mousePos, easyRectangle)){
				level = EASY;
			}
			else if(CheckCollisionPointRec(mousePos, mediumRectangle)){
				level = MEDIUM;
			}
			else if(CheckCollisionPointRec(mousePos, hardRectangle)){
				level = HARD;
			}
			else if(CheckCollisionPointRec(mousePos, extremeRectangle)){
				level = EXTREME;
			}
		}

		if (timer <= 0) 
        {
			if(!gameOver)
			{
				timer = 1.0f / level;
				if(platformCreated == true)
				{
					platformDistance = GetRandomValue(80,300);
					platformCreated = false;
				}
				updatePlatformAndBall();
			}
		}

		if(!gameOver)
		{
			BeginDrawing();
			ClearBackground(BLACK);

			DrawText("SCORE", 390, 40 , 38, WHITE);
			DrawRectangleRounded({380, 80, 150, 60}, 0.3, 6, darkGrey);
			DrawText(TextFormat("%d",score), 400, 90 , 38, WHITE);
			
			DrawText("LEVEL", 395, 250 , 38, WHITE);
			DrawRectangleRounded({380, 290, 150, 160}, 0.3, 6, darkGrey);

			( level == EASY) ?  DrawRectangle(410,320,10,10,GREEN) : DrawRectangleLinesEx(easyRectangle,1,GREEN);
			DrawText("easy", 430, 313 , 20, GREEN);

			( level == MEDIUM) ?  DrawRectangle(410,350,10,10,ORANGE) : DrawRectangleLinesEx(mediumRectangle,1,ORANGE);
			DrawText("medium", 430, 343 , 20, ORANGE);

			( level == HARD) ?  DrawRectangle(410,380,10,10,RED) : DrawRectangleLinesEx(hardRectangle,1,RED);
			DrawText("hard", 430, 373 , 20, RED);

			( level == EXTREME) ?  DrawRectangle(410,410,10,10,darkRed) : DrawRectangleLinesEx(extremeRectangle,1,darkRed);
			DrawText("extreme", 430, 403 , 20, darkRed);

			//DrawRectangle(LUD_BORDER,LUD_BORDER,(WINDOW_WIDTH-LUD_BORDER-RIGHT_BORDER),(WINDOW_HEIGHT - 2 * LUD_BORDER),darkGrey);
			DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);
			
			//origin = {ball.position.width/2, ball.position.height/2};
			//DrawRectangleRounded(ball.position,1.0,15,GREEN);
			DrawTexturePro(balltexture, ballRec,ball.position, origin, rotation, WHITE);

			for(auto platfm : platform)
			{
				DrawTexturePro(brickwall, bricksrcRec, platfm, origin, 0.0f, WHITE);
				//DrawRectangleRounded(platfm, 0.9, 15, RED);
			}

			EndDrawing();
		}
		else
		{
			BeginDrawing();
			ClearBackground(BLACK);
			DrawText("Game Over!", WINDOW_WIDTH / 2 - MeasureText("Game Over!", 20) / 2, WINDOW_HEIGHT / 2 - 10, 20, RED);
			DrawText("Press R to Restart", WINDOW_WIDTH / 2 - MeasureText("Press R to Restart", 20) / 2, WINDOW_HEIGHT / 2 + 20, 20, WHITE);
			EndDrawing();			

			if (IsKeyPressed(KEY_R)) {
				// Restart the game
				platform.clear();
				platform.push_back({100, 200,80,10});
				platform.push_back({200, 300,120,10});
				platform.push_back({150, 500,200,10});
				ball.position = {140,180,20,20};
				ball.falling = false;
				level = EASY;
				score = 0;
				gameOver = false;
				updateScore = false;
			}
		}
	}
}