#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "HazelnutLayer.h" 

namespace Hazel
{
	class HazelnutApp : public Application
	{
	public:
		HazelnutApp() : Application("Hazelnut - Hazel Editor")
		{
			PushLayer(new HazelnutLayer());
		}

		~HazelnutApp() override = default;
	};

	Application* CreateApplication()
	{
		return new HazelnutApp();
	}
}