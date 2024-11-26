#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}
            
        ~TransformComponent() = default;

        operator const glm::mat4&() const { return Transform; }
        operator glm::mat4&() { return Transform; }
        
        glm::mat4 Transform { 1.0f };
    };

    struct SpriteRendererComponent
    {
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
            
        ~SpriteRendererComponent() = default;

        operator const glm::vec4&() const { return Color; }
        operator glm::vec4&() { return Color; }
        
        glm::vec4 Color { 1.0f,1.0f, 1.0f, 1.0f};
    };
}
