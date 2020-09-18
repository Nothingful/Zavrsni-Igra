#include "SpriteRenderer.h"


SpriteRenderer::SpriteRenderer(Shader& shader)
{
    m_shader = shader;
    initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    GLCall(glDeleteVertexArrays(1, &m_QuadVAO));
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
    /* Prepare transformations */
    m_shader.Use();
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

    m_shader.SetUniformMat4f("u_Model", model);

    /* Render textured quad */
    m_shader.SetUniform3f("u_SpriteColor", color);

    GLCall(glActiveTexture(GL_TEXTURE0));
    texture.Bind();

    GLCall(glBindVertexArray(m_QuadVAO));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
    GLCall(glBindVertexArray(0));
}

void SpriteRenderer::initRenderData()
{
    /* Configure VAO/VBO */
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    GLCall(glGenVertexArrays(1, &m_QuadVAO));
    GLCall(glGenBuffers(1, &VBO));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GLCall(glBindVertexArray(m_QuadVAO));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
    //GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(2*sizeof(float))));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    //GLCall(glBindVertexArray(1));
}