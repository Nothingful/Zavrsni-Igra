#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
private:
	/* Render state */
    Shader m_Shader;
    VertexArray* m_QuadVAO;
    /* Initializes and configures the quad's buffer and vertex attributes */
    void initRenderData();
public:
    SpriteRenderer(Shader& shader);
    ~SpriteRenderer();

	/* Renders a defined quad textured with given sprite */
    void DrawSprite(Texture& texture, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f),
        float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));
};