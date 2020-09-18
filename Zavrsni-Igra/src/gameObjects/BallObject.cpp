#include "BallObject.h"

BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true) { }

BallObject::BallObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture sprite)
    : GameObject(position, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true) { }

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth)
{
    /* If not stuck to player board */
    if (!Stuck)
    {
        /* Move the ball */
        Position += Velocity * dt;
        /* then check if outside window bounds and if so, reverse velocity and restore at correct position */
        if (Position.x <= 0.0f)
        {
            Velocity.x = -Velocity.x;
            Position.x = 0.0f;
        }
        else if (Position.x + Size.x >= windowWidth)
        {
            Velocity.x = -Velocity.x;
            Position.x = windowWidth - Size.x;
        }
        if (Position.y <= 0.0f)
        {
            Velocity.y = -Velocity.y;
            Position.y = 0.0f;
        }
    }
    return Position;
}

/* Resets the ball to initial Stuck position (if ball is outside window bounds) */
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    Position = position;
    Velocity = velocity;
    Stuck = true;
}