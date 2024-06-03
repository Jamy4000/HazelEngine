#pragma once

namespace Hazel
{
	enum class RendererAPI
	{
		None		= 0,
		OpenGL		= 1
	};

	class Renderer
	{
	public: //static
		inline static RendererAPI const GetAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;
	};
}