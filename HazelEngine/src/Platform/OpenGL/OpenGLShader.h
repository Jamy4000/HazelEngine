﻿#pragma once

#include "Hazel/Renderer/Shader.h"

#include <glm/glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace Hazel
{
    class OpenGLShader : public Shader
    {
    public:
        explicit OpenGLShader(const std::string& filepath); 
        OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc); 
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;
        
        void SetInt(const char* name, int value) override;
        void SetIntArray(const char* name, int* values, uint32_t count) override;
        void SetFloat(const char* name, float value) override;
        void SetFloat2(const char* name, glm::vec2 value) override;
        void SetFloat3(const char* name, glm::vec3 value) override;
        void SetFloat4(const char* name, glm::vec4 value) override;
        void SetMat4(const char* name, glm::mat4 value) override;
        
		[[nodiscard]] const std::string& GetName() const override { return m_Name; }

        void UploadUniformInt(const std::string& name, int value) const;
        void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count) const;

        void UploadUniformFloat(const std::string& name, float value) const;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        std::string ReadFile(const std::string& filepath);
        static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        
    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}
