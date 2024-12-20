#include "hzpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		HZ_CORE_ASSERT(false, "Unknown Shader Type: {0}!", type)
		return 0;
	}
	
	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_RendererID(-1)
	{
		HZ_PROFILE_FUNCTION()
		
		const std::string source = ReadFile(filepath);
		const auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		const auto lastDot = filepath.rfind('.');
		const auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_RendererID(-1), m_Name(std::move(name))
	{
		HZ_PROFILE_FUNCTION()
		
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		HZ_PROFILE_FUNCTION()
		
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		HZ_PROFILE_FUNCTION()
		
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		HZ_PROFILE_FUNCTION()
		
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const char* name, const int value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const char* name, int* values, uint32_t count)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const char* name, const float value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const char* name, const glm::vec2 value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const char* name, const glm::vec3 value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const char* name, const glm::vec4 value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const char* name, const glm::mat4 value)
	{
		HZ_PROFILE_FUNCTION()
		
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, const int* values, const uint32_t count) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, static_cast<GLint>(count), values);
	}
	
	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		// TODO glGetUniformLocation in this specific method is extremely slow, we may wanna cache it or find a better way
		// https://android-developers.googleblog.com/2015/04/game-performance-explicit-uniform.html
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		HZ_PROFILE_FUNCTION()
		
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			const size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], static_cast<std::streamsize>(size));
				in.close();
			}
			else
			{
				HZ_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		HZ_PROFILE_FUNCTION()
		
		std::unordered_map<GLenum, std::string> shaderSources;

		const auto typeToken = "#type";
		const size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		
		while (pos != std::string::npos)
		{
			const size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax Error!")

			const size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified!")

			const size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error")
			
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line
			const std::string shader = pos == std::string::npos ?
				source.substr(nextLinePos) :
				source.substr(nextLinePos, pos - nextLinePos);
			
			shaderSources[ShaderTypeFromString(type)] = shader;
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		HZ_PROFILE_FUNCTION()
		
		const GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders per file for now!")
		
		std::array<GLuint, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		
		for (const auto& [fst, snd] : shaderSources)
		{
			const GLenum type = fst;
			const std::string& source = snd;

			// Create an empty shader handle
			const GLuint shader = glCreateShader(type);
			
			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				HZ_CORE_ASSERT(false, "Shader compilation failed. Error:\n{0}", infoLog.data())

				// In this simple program, we'll just leave
				break;
			}
			
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}
		
		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (const auto id : glShaderIDs)
			{
				// Don't leak shaders either.
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			HZ_CORE_ASSERT(false, "Shader Link failed. Error:\n{0}", infoLog.data())

			// In this simple program, we'll just leave
			return;
		}

		for (const auto id : glShaderIDs)
		{
			// Always detach shaders after a successful link.
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		
		m_RendererID = program;
	}
}
