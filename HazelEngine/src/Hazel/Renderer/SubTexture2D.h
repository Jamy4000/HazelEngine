#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Hazel
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        [[nodiscard]] Ref<Texture2D> GetTexture()           const { return m_Texture; }
        [[nodiscard]] const glm::vec2* GetTextureCoordinates()   const { return m_TexCoords; }

        static Ref<SubTexture2D> CreateFromCoordinates(const Ref<Texture2D>& texture,
            const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.0f, 1.0f });
        
    private:
        const Ref<Texture2D>& m_Texture;
        const glm::vec2 m_TexCoords[4];
    };
}
