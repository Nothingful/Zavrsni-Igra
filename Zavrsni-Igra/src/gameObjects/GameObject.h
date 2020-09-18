#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
    /* Object state */
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    float Rotation;
    bool IsSolid;
    bool Destroyed;
	
    /* Render state */
    Texture Sprite;
	
    /* Constructors */
    GameObject();
    GameObject(glm::vec2 position, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	
    virtual void Draw(SpriteRenderer& renderer);
};