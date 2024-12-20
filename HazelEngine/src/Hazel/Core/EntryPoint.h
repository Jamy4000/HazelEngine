#pragma once
#include "Hazel/Core/Base.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

inline int main(int argc, char** argv)
{
	Hazel::Log::Init();

	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile_Startup.json")
	const auto app = Hazel::CreateApplication();
	HZ_PROFILE_END_SESSION()

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile_Runtime.json")
	app->Run();
	HZ_PROFILE_END_SESSION()
	
	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile_Shutdown.json")
	delete app;
	HZ_PROFILE_END_SESSION()
}

#endif