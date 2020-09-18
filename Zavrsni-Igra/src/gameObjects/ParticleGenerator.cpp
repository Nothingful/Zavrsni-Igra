#include "ParticleGenerator.h"


ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
    : m_Shader(shader), m_Texture(texture), m_Amount(amount)
{
    init();
}

void ParticleGenerator::Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset)
{
    // Add new particles */
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = firstUnusedParticle();
        respawnParticle(m_Particles[unusedParticle], object, offset);
    }
    /* Update all particles */
    for (unsigned int i = 0; i < m_Amount; ++i)
    {
        Particle& p = m_Particles[i];
        p.Life -= deltaTime; // Reduce life
        if (p.Life > 0.0f) // If particle alive
        {	
            p.Position -= p.Velocity * deltaTime;
            p.Color.a -= deltaTime * 2.5f;
        }
    }
}

/* Render all particles */
void ParticleGenerator::Draw()
{
    /* Use additive blending to give it a 'glow' effect */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_Shader.Bind();
    for (Particle particle : m_Particles)
    {
        if (particle.Life > 0.0f) // Particle alive
        {
            m_Shader.SetUniform2f("u_Offset", particle.Position);
            m_Shader.SetUniform4f("u_Color", particle.Color);
            m_Texture.Bind();
        	
            m_VAO->Bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
            m_VAO->Unbind();
        }
    }
    /* Reset to default blending mode */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    m_VAO = new VertexArray();
	
    /* Set up mesh and attribute properties */
    float particleQuad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    VertexBuffer* VBO = new VertexBuffer(particleQuad, sizeof(particleQuad));

    m_VAO->AddBuffer(*VBO, 4);

    m_VAO->Unbind();

    /* Create "amount" of default particle instances */
    for (unsigned int i = 0; i < m_Amount; ++i)
    {
        m_Particles.push_back(Particle());
    }
}

unsigned int lastUsedParticle = 0;
/* Stores the index of the last particle used (for quick access to next dead particle) */
unsigned int ParticleGenerator::firstUnusedParticle()
{
    /* First search from last used particle, this will usually return almost instantly */
    for (unsigned int i = lastUsedParticle; i < m_Amount; ++i) {
        if (m_Particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (m_Particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    /* All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved) */
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
    float random = ((rand() % 100) - 50) / 10.0f; // Position variation
    float rColor = 0.5f + ((rand() % 100) / 100.0f); // Brightness variation
	
    particle.Position = object.Position + random + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    particle.Velocity = object.Velocity * 0.1f;
}