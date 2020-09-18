#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	/* stb_image loading buffer */
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	/* Texture Format */
	unsigned int InternalFormat; // format of texture object
	unsigned int ImageFormat; // format of loaded image
	/* Texture configuration */
	unsigned int Wrap_S; // Wrapping mode on S axis
	unsigned int Wrap_T; // Wrapping mode on T axis
	unsigned int Filter_Min; // Filtering mode if texture pixels < screen pixels
	unsigned int Filter_Max; // Filtering mode if texture pixels > screen pixels

	Texture();
	Texture(const std::string& path, bool alpha = true);
	~Texture();

	void Generate(const std::string& path, bool alpha = true);

	void Bind() const;
	//void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	void Delete() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};