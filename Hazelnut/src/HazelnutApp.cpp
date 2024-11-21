#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h" 

namespace Hazel
{
	class HazelnutApp : public Application
	{
	public:
		HazelnutApp() : Application("Hazelnut - Hazel Editor")
		{
			PushLayer(new EditorLayer());
		}

		~HazelnutApp() override = default;
	};

	Application* CreateApplication()
	{
		return new HazelnutApp();
	}
}