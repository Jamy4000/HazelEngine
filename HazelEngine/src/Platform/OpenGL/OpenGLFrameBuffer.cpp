#include "hzpch.h"
#include "OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Hazel
{
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specifications) :
        m_RendererID(-1), m_ColorAttachments(-1), m_Specification(specifications)
    {
        Invalidate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
    }

    void OpenGLFrameBuffer::Invalidate()
    {
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachments);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachments);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<GLsizei>(m_Specification.Width),
            static_cast<GLsizei>(m_Specification.Height), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachments, 0);

        // Depth buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Specification.Width),
            static_cast<GLsizei>(m_Specification.Height));
        /*
         *Same as line above
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<GLsizei>(m_Specification.Width),
            static_cast<GLsizei>(m_Specification.Height), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
        */
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebugger is incomplete");
        
        // Unbinding
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }

    void OpenGLFrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
