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
        OpenGLShader(const std::string& filepath); 
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc); 
        ~OpenGLShader() override;

        void Bind() const override;
        void Unbind() const override;

        void UploadUniformInt(const std::string& name, int value) const;

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
    };
}
