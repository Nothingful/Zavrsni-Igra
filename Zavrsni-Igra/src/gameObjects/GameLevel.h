#pragma once

#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "gameObjects/GameObject.h"
#include "gameObjects/SpriteRenderer.h"
#include "ResourceManager.h"

class GameLevel
{
public:
    /* Level state */
    std::vector<GameObject> Bricks;

    GameLevel() { } // user-provided default constructor
	
    /* Load level from file */
    void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
    /* Render level */
    void Draw(SpriteRenderer& renderer);
    /* check if the level is completed (all non-solid tiles are destroyed) */
    bool IsCompleted();
private:
    /* Initialize level from tile data */
    void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};