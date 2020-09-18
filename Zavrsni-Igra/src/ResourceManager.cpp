#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image/stb_image.h"

/* Instantiate static variables */
std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name)
{
    Shader shader;
    shader.Compile(vShaderFile, fShaderFile);
    Shaders[name] = shader;
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, std::string name, bool alpha)
{
    Texture texture;
    texture.Generate(file, alpha);
    Textures[name] = texture;
    return Textures[name];
}

Texture& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    /* Delete all shaders */
    for (auto iter : Shaders)
    {
        iter.second.Delete();
    }
    /* Delete all textures */
    for (auto iter : Textures)
    {
	    iter.second.Delete();
    }
}

/*
Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile)
{
    Shader shader;
    shader.Compile(vShaderFile, fShaderFile);
    return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    Texture texture;
    texture.Generate(file, alpha);
    return texture;
}
*/