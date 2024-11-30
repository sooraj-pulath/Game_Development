#include "snake.h"
#include <iostream>

namespace SnakeGame {

	const Color darkGrey{26, 31, 40, 255};
	const Color darkRed{ 139, 0, 0, 255 };

	Snake::Snake()
	{
		std::cout << "Snake object created ::" << std::endl;
		snakeBody.push_back({GRID_WIDTH/2, GRID_HEIGHT/2});
		snakeBody.push_back({GRID_WIDTH/2 + 1,GRID_HEIGHT/2});
		snakeBody.push_back({GRID_WIDTH/2 + 2,GRID_HEIGHT/2});
		easyRectangle = {410, 320, 10, 10};
		mediumRectangle = {410, 350, 10, 10};
		hardRectangle = {410, 380, 10, 10};
		extremeRectangle = {410, 410, 10, 10};
		snakeDirection = RIGHT;
		level = EASY;
	}

	Snake::~Snake()
	{
		// Snake Game End
	}
	Vector2 Snake::getFoodPosition()
	{
		Vector2 pos;
		bool valid;

		do
		{
			valid = true;
			pos.x = GetRandomValue(1,GRID_WIDTH-1);
			pos.y = GetRandomValue(1,GRID_HEIGHT-1);

			// check if the pos is in snake body
			for(auto point : snakeBody)
			{
				if((point.x == pos.x) && (point.y == pos.y)){
					valid = false;
					break;
				}
			}
		}while(!valid);
		return pos;
	}

	void Snake::setupAudio()
	{
		InitAudioDevice(); // Initialize audio device

		if(IsAudioDeviceReady())
		{
			bgMusic = LoadMusicStream("sounds/music.mp3");

			eatSound = LoadSound("sounds/eat.mp3");

			collisionSound = LoadSound("sounds/hit.mp3");
		}
	}

	void Snake::run()
	{
		InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake Game");
		SetTargetFPS(60);

		foodPosition = getFoodPosition();
		bool gameOver = false;

		PlayMusicStream(bgMusic);
		uint32_t frameCount = 0;
		float timer = 1.0f / level;
		score  = 0;

		while(!WindowShouldClose())
		{

			UpdateMusicStream(bgMusic); // Update audio buffer
	        timer -= GetFrameTime();  // Subtract frame time from timer
	        //std::cout << " Food Position ::"<< GetFrameTime()  <<" " << timer<< std::endl; 

			if(IsKeyPressed(KEY_UP) && (snakeDirection != DOWN)) snakeDirection = UP;
			if(IsKeyPressed(KEY_DOWN) && (snakeDirection != UP)) snakeDirection = DOWN;
			if(IsKeyPressed(KEY_RIGHT) && (snakeDirection != LEFT)) snakeDirection = RIGHT;
			if(IsKeyPressed(KEY_LEFT) && (snakeDirection != RIGHT)) snakeDirection = LEFT;

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

				timer = 1.0f / level;
				int index = snakeBody.size() - 1;
				Vector2 newPos;
				switch (snakeDirection)
				{
					case UP :
						newPos = ((snakeBody[index].y-1) < 1) ? (Vector2){snakeBody[index].x, GRID_HEIGHT} : (Vector2){snakeBody[index].x, snakeBody[index].y-1};
						break;
					case DOWN :
						newPos = ((snakeBody[index].y+1) > GRID_HEIGHT) ? (Vector2){snakeBody[index].x, 1} : (Vector2){snakeBody[index].x, snakeBody[index].y+1};
						break;
					case LEFT :
						newPos = ((snakeBody[index].x-1) < 1) ? (Vector2){GRID_WIDTH, snakeBody[index].y} : (Vector2){snakeBody[index].x-1, snakeBody[index].y};
						break;
					case RIGHT :
						newPos = ((snakeBody[index].x+1) > GRID_WIDTH) ? (Vector2){1, snakeBody[index].y} : (Vector2){snakeBody[index].x+1, snakeBody[index].y};
						break;
				}


				if(!gameOver)
				{
					//check for collision
					for(auto pos : snakeBody)
					{
						if((pos.x == newPos.x) && (pos.y == newPos.y)){
							std::cout << " :: Collision ::" << std::endl;
							PlaySound(collisionSound);
							gameOver = true;
						}
					}

					snakeBody.push_back(newPos);
					index = snakeBody.size()-1;
					//check for snake eat food
					if((snakeBody[index].x == foodPosition.x) && (snakeBody[index].y == foodPosition.y)){
						PlaySound(eatSound);
						foodPosition = getFoodPosition();
						score++;
					}
					else{
						snakeBody.erase(snakeBody.begin());
					}
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

				DrawRectangle(LUD_BORDER,LUD_BORDER,(WINDOW_WIDTH-LUD_BORDER-RIGHT_BORDER),(WINDOW_HEIGHT - 2 * LUD_BORDER),darkGrey);
				int head = 0;

				for(auto points : snakeBody)
				{
					head++;
					if(head == snakeBody.size())
					{
						DrawCircle(points.x * GRID_SIZE + GRID_SIZE/2 , points.y * GRID_SIZE + GRID_SIZE/2, GRID_SIZE/2, ORANGE);
						//DrawRectangle(points.x * GRID_SIZE, points.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLUE);
					}
					else
					{
						DrawRectangle(points.x * GRID_SIZE, points.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, GREEN);
					}
				}

				DrawRectangle(foodPosition.x * GRID_SIZE, foodPosition.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);

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
					snakeBody.clear();
					snakeBody.push_back({GRID_WIDTH/2, GRID_HEIGHT/2});
					snakeBody.push_back({GRID_WIDTH/2 + 1,GRID_HEIGHT/2});
					snakeBody.push_back({GRID_WIDTH/2 + 2,GRID_HEIGHT/2});
					snakeDirection = RIGHT;
					foodPosition = getFoodPosition();
					gameOver = false;
				}
			}
		}
	}
}