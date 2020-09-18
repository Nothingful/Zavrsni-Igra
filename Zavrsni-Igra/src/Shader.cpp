#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader() : m_RendererID(0)
{
    
}

void Shader::Compile(const char* filepathVertex, const char* filepathFragment)
{
    m_VertexFilePath = filepathVertex;
    m_FragmentFilePath = filepathFragment;
    ShaderProgramSource source = ParseShaders(filepathVertex, filepathFragment);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& filepathVertex, const std::string& filepathFragment) : m_VertexFilePath(filepathVertex), m_FragmentFilePath(filepathFragment), m_RendererID(0)
{
    ShaderProgramSource source = ParseShaders(filepathVertex, filepathFragment);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
    //SHADER PRINTOUT
    //std::cout << "VERTEX" << std::endl;
    //std::cout << source.VertexSource << std::endl;
    //std::cout << "FRAGMENT" << std::endl;
    //std::cout << source.FragmentSource << std::endl;
}

Shader::~Shader()
{
    //GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSource Shader::ParseShaders(const std::string& filepathVertex, const std::string& filepathFragment)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    /* Ensure ifstream objects can throw exceptions: */
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        /* Open files */
        vShaderFile.open(filepathVertex);
        fShaderFile.open(filepathFragment);
        std::stringstream vShaderStream, fShaderStream;
        /* Read file's buffer contents into streams */
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        /* Close file handlers */
        vShaderFile.close();
        fShaderFile.close();
        /* Convert stream into string */
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return { vertexCode, fragmentCode };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
    	/*
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
    	*/
        GLsizei log_length = 0;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length));
        GLchar message[1024];
        GLCall(glGetShaderInfoLog(id, 1024, &log_length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	/* Bind shaders to program and validate them */
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

	/* Cleanup */
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

Shader& Shader::Use()
{
    GLCall(glUseProgram(m_RendererID));
    return *this;
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::Delete() const
{
    Unbind();
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::SetUniform1i(const std::string& name, int value, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform1i(location, value));
}

void Shader::SetUniform1f(const std::string& name, float value, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform1f(location, value));
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& value, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform2f(location, value.x, value.y));
}

void Shader::SetUniform2f(const std::string& name, float x, float y, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform2f(location, x, y));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void Shader::SetUniform3f(const std::string& name, float x, float y, float z, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform3f(location, x, y, z));
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, value.x, value.y, value.z, value.w));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniform4f(location, x, y, z, w));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix, bool useShader)
{
    if (useShader) Bind();
    int location = GetUniformLocation(name);
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
}


int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
	return location;
}