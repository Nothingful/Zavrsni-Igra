#pragma once

#include <map>
#include <string>

#include <gl/glew.h>

#include "texture.h"
#include "shader.h"

class ResourceManager
{
public:
    /* Resource storage */
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture> Textures;
	
    /* Load and generate a shader program from file */
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);
    /* Retrieves a stored shader */
    static Shader& GetShader(std::string name);
    /* Load and generate a texture from file */
    static Texture LoadTexture(const char* file, std::string name, bool alpha = true);
    /* Retrieves a stored texture */
    static Texture& GetTexture(std::string name);
	
    /* properly de-allocates all loaded resources */
    static void Clear();
private:
    /* Private constructor, because of static implementations */
    ResourceManager() { }

    //static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
	
    //static Texture loadTextureFromFile(const char* file, bool alpha);
};
