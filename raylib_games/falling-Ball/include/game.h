#pragma once
#include "raylib.h"
#include <stdint.h>
#include <vector>

#define WINDOW_WIDTH  550
#define WINDOW_HEIGHT 600
#define LUD_BORDER 10
#define RIGHT_BORDER 190

typedef enum Level
{
	EASY = 10,
	MEDIUM = 20,
	HARD =40,
	EXTREME = 60,
}Level;

class Game
{
	struct Ball
	{
		Rectangle position;
		bool falling;
	};

public:
	Game();
	~Game();
	void setupAudio();
	void run();
	void updatePlatformAndBall();
	void generatePlatform();
	void checkBallFall();
private:
	std::vector<Rectangle> platform;
	struct Ball ball;
	Level level;
	Music bgMusic;
	Sound landingSound;
	Sound failSound;
	uint32_t score;
	Rectangle easyRectangle;
	Rectangle mediumRectangle;
	Rectangle hardRectangle;
	Rectangle extremeRectangle;
	uint32_t platformDistance;
	bool platformCreated;
	bool gameOver;
	bool updateScore;
};