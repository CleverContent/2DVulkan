#include "Engine.h"

Engine::Engine(EngineStartFlags flags)
{
	EventControllerFlags ecf = {};
	eventController = EventController{ ecf };

	ObjectControllerFlags ocf{};
	objectController = ObjectController{ ocf, eventController.EventData()};

	RenderControllerFlags rcf{flags.windowSize};
	renderController = RenderController{rcf, objectController.getAllWorldsObjectsData(), eventController.EventData()};

	PhysicsControllerFlags PCF{  };
	physicsController = PhysicsController{ PCF };

	MagicControllerFlags MCF{  };
	magicController = MagicController{MCF};


	objectController.Init();
	eventController.Init(CoreVulkanData::getInstance()->glfwWindow);
	physicsController.Init();
	magicController.Init();
}

Engine::~Engine()
{

}

void Engine::Update()
{
	startTime = std::chrono::high_resolution_clock::now();

	eventController.Update();

	renderController.StartUpdate();

	WindowSwitcher();

	objectController.Update();

	magicController.Update();

	physicsController.Update();

	renderController.FinishUpdate();

	auto now = std::chrono::high_resolution_clock::now();
	PublicVulkanData::getInstance()->LastFrameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count() / 1000000000.0f;
}

void Engine::WindowSwitcher()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(("Active Window: " + WindowStringNames.at(PublicVulkanData::getInstance()->currentActiveWindow)).c_str()))
		{
			if (ImGui::MenuItem("World"))
			{
				PublicVulkanData::getInstance()->currentActiveWindow = WORLD;
			}
			if (ImGui::MenuItem("Spell Creator"))
			{
				PublicVulkanData::getInstance()->currentActiveWindow = SPELLCREATOR;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

EngineData Engine::GetEngineData()
{
	return EngineData();
}

bool Engine::DidEngineEnd()
{
	return renderController.HasWindowClosed();
}
