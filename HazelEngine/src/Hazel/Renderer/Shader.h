#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

namespace Hazel
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual void SetInt(const char* name, int value) = 0;
		virtual void SetIntArray(const char* name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const char* str, float value) = 0;
		virtual void SetFloat2(const char* str, glm::vec2 value) = 0;
		virtual void SetFloat3(const char* str, glm::vec3 value) = 0;
		virtual void SetFloat4(const char* str, glm::vec4 value) = 0;
		virtual void SetMat4(const char* str, glm::mat4 value) = 0;

		[[nodiscard]] virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const Ref<Shader>& shader);
		void Add(const Ref<Shader>& shader);
		
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		[[nodiscard]] bool Exists(const std::string& name) const;

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
