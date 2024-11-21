#pragma once

#include "Hazel/Core/Base.h"

namespace Hazel
{
    struct FrameBufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Sample = 1;

        bool SwapChainTarget = false;
    };

    class FrameBuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        [[nodiscard]] virtual uint32_t GetColorAttachmentRendererID() const = 0;
        
        [[nodiscard]] virtual const FrameBufferSpecification& GetSpecifications() const = 0;
        
        static Ref<FrameBuffer> Create(const FrameBufferSpecification& specifications);
    };
}