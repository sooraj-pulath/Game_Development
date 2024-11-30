#pragma once
#include "raylib.h"
#include <string>
#include <vector>

#define GAMEFPS 60
#define GRID_SIZE 10
#define WINDOW_WIDTH  550
#define WINDOW_HEIGHT 600
#define LUD_BORDER 10
#define RIGHT_BORDER 190
#define GRID_WIDTH 35
#define GRID_HEIGHT 58

namespace SnakeGame {

    typedef enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    }Direction;

    typedef enum Level
    {
        EASY = 4,
        MEDIUM = 8,
        HARD =16,
        EXTREME = 40,
    }Level;

    class Snake {

    public:
        Snake();
        ~Snake();
        void run();
        void setupAudio();

    private:
        Vector2 getFoodPosition();
        std::vector<Vector2> snakeBody;
        Direction snakeDirection;
        Level level;
        Vector2 foodPosition;
        Music bgMusic;
        Sound eatSound;
        Sound collisionSound;
        uint32_t score;
        Rectangle easyRectangle;
        Rectangle mediumRectangle;
        Rectangle hardRectangle;
        Rectangle extremeRectangle;

    };

}