#pragma once
#include "Hazel/Core/Core.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

inline int main(int argc, char** argv)
{
	Hazel::Log::Init();
	const auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif