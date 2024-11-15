#include "hzpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Hazel
{
    OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
        : m_Width(width), m_Height(height), m_RendererID(-1), m_InternalFormat(GL_RGBA8), m_DataFormat(GL_RGBA)
    {
		HZ_PROFILE_FUNCTION()
		
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, static_cast<int>(width), static_cast<int>(height));

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path) :
        m_Path(path), m_RendererID(-1)
    {
		HZ_PROFILE_FUNCTION()
		
        int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
        stbi_uc* data;
		{
		    HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)")
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
        
        HZ_CORE_ASSERT(data, "Failed to load image!")
        
        m_Width = width;
        m_Height = height;

        m_InternalFormat = 0;
        m_DataFormat = 0;
        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        }

        HZ_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!") 
        
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
		HZ_PROFILE_FUNCTION()
		
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
		HZ_PROFILE_FUNCTION()
		
        const uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
        HZ_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Data must be entire Texture!")
        
        glTextureSubImage2D(m_RendererID, 0, 0, 0, static_cast<int>(m_Width), static_cast<int>(m_Height),
            m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(const uint32_t slot) const
    {
		HZ_PROFILE_FUNCTION()
		
        glBindTextureUnit(slot, m_RendererID);
    }

}