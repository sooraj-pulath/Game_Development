#include <stdio.h>
#include "snake.h"

using namespace SnakeGame;

int main()
{
	Snake snakeGame;
	snakeGame.setupAudio();
	snakeGame.run();

	return 0;
}