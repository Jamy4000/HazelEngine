#include "hzpch.h"
#include "FrameBuffer.h"

#include "Hazel/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel
{
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specifications)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
                HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;

            case RendererAPI::API::OpenGL:
                return CreateRef<OpenGLFrameBuffer>(specifications);
        }

        HZ_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }

    
}
