#pragma once

#include <iostream>
#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "BallObject.h"

/* DEBUGING */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

/* Current state of the game */
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

/* Represents the four possible directions - used for collision */
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

/*
 Defines a Collision typedef that represents collision data.
 <collision?, what direction?, difference vector center - closest point>
 */
typedef std::tuple<bool, Direction, glm::vec2> Collision;

/* Game state and functionality */
class Game
{
private:
	/* Collision detection */
    bool CheckCollision(GameObject& a, GameObject& b);
    Collision CheckCollision(BallObject& circle, GameObject& box);

    /* Calculates which direction a vector is facing (N,E,S or W) */
    Direction VectorDirection(glm::vec2 target);
public:
    /* Game state */
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int Level;
	
    Game(unsigned int width, unsigned int height);
    ~Game();
	
    /* Load all shaders/textures/levels */
    void Init();
	
    /* Game loop */
    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();
    void DoCollisions();

    /* Reset functions */
    void ResetLevel();
    void ResetPlayer();
};