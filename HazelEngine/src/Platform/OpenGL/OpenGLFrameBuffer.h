#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel
{
    class OpenGLFrameBuffer final : public FrameBuffer
    {
    public:
        OpenGLFrameBuffer(const FrameBufferSpecification& specifications);
        ~OpenGLFrameBuffer();

        void Invalidate();

        void Bind() override;
        void Unbind() override;
        
        [[nodiscard]] uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachments; }
        
        [[nodiscard]] const FrameBufferSpecification& GetSpecifications() const override { return m_Specification; }
        
    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachments, m_DepthAttachment;
        FrameBufferSpecification m_Specification;
    };
}