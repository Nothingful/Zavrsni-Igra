#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	std::string m_VertexFilePath;
	std::string m_FragmentFilePath;
	unsigned int m_RendererID;
	
	/* Caching for uniforms */
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader();
	Shader(const std::string& filepathVertex, const std::string& filepathFragment);
	~Shader();

	void Compile(const char* filepathVertex, const char* filepathFragment);

	void Bind() const;
	/* Sets the current shader as active and returns it, alternative to Bind() */
	Shader& Use();
	void Unbind() const;
	void Delete() const;

	/* Set uniforms */
	void SetUniform1i(const std::string& name, int value, bool useShader = false);
	void SetUniform1f(const std::string& name, float value, bool useShader = false);
	void SetUniform2f(const std::string& name, const glm::vec2& value, bool useShader = false);
	void SetUniform2f(const std::string& name, float x, float y, bool useShader = false);
	void SetUniform3f(const std::string& name, const glm::vec3& value, bool useShader = false);
	void SetUniform3f(const std::string& name, float x, float y, float z, bool useShader = false);
	void SetUniform4f(const std::string& name, const glm::vec4& value, bool useShader = false);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w, bool useShader = false);
	
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix, bool useShader = false);
private:
	ShaderProgramSource ParseShaders(const std::string& filepathVertex, const std::string& filepathFragment);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};