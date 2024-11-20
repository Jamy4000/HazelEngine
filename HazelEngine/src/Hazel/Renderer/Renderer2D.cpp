#include "hzpch.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TextureIndex;
        float TillingFactor;
    };
    
    struct Renderer2DData
    {
        const uint32_t MaxQuads = 10000;
        const uint32_t MaxVertices = MaxQuads * 4;
        const uint32_t MaxIndices = MaxQuads * 6;
        static constexpr uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
        
        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBuggerPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 1 for white texture, ie. only color

        glm::vec4 QuadVertexPositions[4];
    };

    static Renderer2DData s_Data;
    
    void Renderer2D::Init()
    {
		HZ_PROFILE_FUNCTION()
		
        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        s_Data.QuadVertexBuffer->SetLayout(
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float, "a_TextureIndex" },
            { ShaderDataType::Float, "a_TillingFactor" }
        });
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        const auto quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        
        const Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
        delete[] quadIndices;
        
        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        static uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];
        constexpr auto maxTextureSlots = static_cast<int32_t>(s_Data.MaxTextureSlots);
        for (int32_t i = 0; i < maxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        
        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

        // Set all texture slots to 0
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
		HZ_PROFILE_FUNCTION()
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
		HZ_PROFILE_FUNCTION()
		
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBuggerPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1; // 0 is for white texture ie. plain color quad
    }

    void Renderer2D::EndScene()
    {
		HZ_PROFILE_FUNCTION()

        const long long dataSize = reinterpret_cast<uint8_t*>(s_Data.QuadVertexBuggerPtr) -
            reinterpret_cast<uint8_t*>(s_Data.QuadVertexBufferBase); 
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);
        
        Flush();
    }

    void Renderer2D::Flush()
    {
        // Bind Textures
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, static_cast<int>(s_Data.QuadIndexCount));
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
		HZ_PROFILE_FUNCTION()

        constexpr float textureIndex = 0.0f; // White Texture
        constexpr float tillingFactor = 1.0f; // White Texture

        const auto unitMatrix = glm::mat4(1.0f);
        const glm::mat4 transform = glm::translate(unitMatrix, position)
            * glm::scale(unitMatrix, { size.x, size.y, 1.0f });
        
        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
        float tillingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
        float tillingFactor, const glm::vec4& tintColor)
    {
		HZ_PROFILE_FUNCTION()

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i] == *texture.get())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }
        
        if (textureIndex == 0.0f)
        {
            textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }
        
        const auto unitMatrix = glm::mat4(1.0f);
        const glm::mat4 transform = glm::translate(unitMatrix, position)
            * glm::scale(unitMatrix, { size.x, size.y, 1.0f });
        
        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation,
        const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
        const glm::vec4& color)
    {
        HZ_PROFILE_FUNCTION()

        constexpr float textureIndex = 0.0f; // White Texture
        constexpr float tillingFactor = 1.0f; // White Texture

        const auto unitMatrix = glm::mat4(1.0f);
        constexpr auto rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); 
        const glm::mat4 transform = glm::translate(unitMatrix, position)
            * glm::rotate(unitMatrix, rotation, rotationAxis)
            * glm::scale(unitMatrix, { size.x, size.y, 1.0f });
        
        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBuggerPtr->Color = color;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadIndexCount += 6;
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation,
        const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tillingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation,
        const Ref<Texture2D>& texture, float tillingFactor, const glm::vec4& tintColor)
    {
		HZ_PROFILE_FUNCTION()

        float textureIndex = 0.0f;
        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i] == *texture.get())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }
        
        if (textureIndex == 0.0f)
        {
            textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++;
        }

        const auto unitMatrix = glm::mat4(1.0f); 
        constexpr auto rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f); 
        glm::mat4 transform = glm::translate(unitMatrix, position)
            * glm::rotate(unitMatrix, glm::radians(rotation), rotationAxis)
            * glm::scale(unitMatrix, { size.x, size.y, 1.0f });
        
        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadVertexBuggerPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBuggerPtr->Color = tintColor;
        s_Data.QuadVertexBuggerPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBuggerPtr->TextureIndex = textureIndex;
        s_Data.QuadVertexBuggerPtr->TillingFactor = tillingFactor;
        s_Data.QuadVertexBuggerPtr++;

        s_Data.QuadIndexCount += 6;
    }
}
