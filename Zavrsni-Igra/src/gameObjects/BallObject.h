#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Texture.h"


class BallObject : public GameObject
{
public:
    /* Ball state */	
    float Radius;
    bool Stuck;

    BallObject();
    BallObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture sprite);

    /* Moves the ball, keeps it constrained within the window bounds (except bottom edge), returns new position */
    glm::vec2 Move(float dt, unsigned int window_width);

    /* Resets the ball to original state with given position and velocity */
    void Reset(glm::vec2 position, glm::vec2 velocity);
};
