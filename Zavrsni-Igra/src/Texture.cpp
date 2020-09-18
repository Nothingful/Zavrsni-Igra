#include "Texture.h"

#include <iostream>

#include "stb_image/stb_image.h"

Texture::Texture()
	: m_RendererID(0), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0),
	ImageFormat(GL_RGBA), InternalFormat(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
}

void Texture::Generate(const std::string& path, bool alpha)
{
	m_FilePath = path;

	/* stbi flip verticaly on load if textures weird */
	//stbi_set_flip_vertically_on_load(1);
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	if (!alpha)
	{
		ImageFormat = GL_RGB;
		InternalFormat = GL_RGB;
	}

	/* Define texture scaling and wrapping settings */
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T));

	if (m_LocalBuffer)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, m_Width, m_Height, 0, ImageFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(m_LocalBuffer);
}

Texture::Texture(const std::string& path, bool alpha)
	: m_RendererID(0), m_FilePath(path) ,m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0),
	ImageFormat(GL_RGBA), InternalFormat(GL_RGBA), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	/* stbi flip verticaly on load if textures weird */
	//stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	
	if (!alpha)
	{
		ImageFormat = GL_RGB;
		InternalFormat = GL_RGB;
	}

	/* Define texture scaling and wrapping settings */
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T));

	if (m_LocalBuffer)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, m_Width, m_Height, 0, ImageFormat, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	//GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

/*
void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
*/

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete() const
{
	Unbind();
	GLCall(glDeleteTextures(1, &m_RendererID));
}
