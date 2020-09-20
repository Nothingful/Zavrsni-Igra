#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
    m_Shader = shader;
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    //GLCall(glDeleteVertexArrays(1, &m_QuadVAO));
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    /* Prepare transformations */
    m_Shader.Bind();
	
    glm::mat4 model = glm::mat4(1.0f);

    /* First translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order) */
    model = glm::translate(model, glm::vec3(position, 0.0f));
    /* Move origin of rotation to center of quad */
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    /* Then rotate */
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    /* Move origin back */
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    /* Last scale */
    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_Shader.SetUniformMat4f("u_Model", model);

    /* Render textured quad */
    m_Shader.SetUniform3f("u_SpriteColor", color);

    //GLCall(glActiveTexture(GL_TEXTURE0)); // Setting texture slot 0
    texture.Bind();

    m_QuadVAO->Bind();

	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

    m_QuadVAO->Unbind();
}

void SpriteRenderer::initRenderData()
{
    /* Configure VAO/VBO */
    m_QuadVAO = new VertexArray();

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    VertexBuffer* VBO;
	VBO = new VertexBuffer(vertices, sizeof(vertices));

    m_QuadVAO->AddBuffer(*VBO, 4);
}