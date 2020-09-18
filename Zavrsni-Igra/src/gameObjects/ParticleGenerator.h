#pragma once

#include <vector>

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"


/* Single particle and its state */
struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


/*
ParticleGenerator acts as a container for rendering a large number of 
particles by repeatedly spawning and updating particles and killing 
them after a given amount of time.
*/
class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture texture, unsigned int amount);
    void Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    /* Particles state */
    std::vector<Particle> m_Particles;
    unsigned int m_Amount;
	
    /* Render state */
    Shader m_Shader;
    Texture m_Texture;
    VertexArray* m_VAO;
	
    /* Initialize buffer and vertex attributes */
    void init();
	
    /* Returns the first Particle index that's currently unused (Life <= 0.0f) or 0 if no particle is currently inactive */
    unsigned int firstUnusedParticle();
	
    /* Respawns particle */
    void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};