#include "ObjectController.h"

ObjectController::ObjectController()
{
}

ObjectController::ObjectController(ObjectControllerFlags flags, KeyInputData* eventData)
{
	this->eventData = eventData;

	for (int i = 0; i < Windows::UNKNOWN; i++)
	{
		worldObjectsData.insert({ (Windows)i, WorldObjectsData{} });
	}

}

ObjectController::~ObjectController()
{

}

void ObjectController::Init()
{
	vulkanData = CoreVulkanData::getInstance();
	publicVulkanData = PublicVulkanData::getInstance();

	publicVulkanData->fontData.glyphTexture = TextureInit::CreateTextureForBezierLocation();
	publicVulkanData->fontData.texture = TextureInit::CreateTextureForFont();

	glm::vec2 offset = { 20, 150 };

	glm::vec2 scale = { (vulkanData->windowSize.x - offset.x) / 26.0f, (vulkanData->windowSize.y - offset.y) / 26.0f };
	int counter = 0;
	for (auto [key ,value] : publicVulkanData->fontData.charsToGlyphIndex)
	{
		GameObject gameObject{};
		gameObject.rotation = 0;
		gameObject.scale = scale;
		gameObject.color = { 0.0f, 0.0f, 0.0f };


		gameObject.position = { ((counter % 26) * scale.x) + offset.x, ((counter / 26) * scale.y) + offset.y / 2 };
		gameObject.rotation = 0;
		gameObject.color = { 0.0, 0.0, 0.0 };
		gameObject.vulkanData.pushConstants.glpyhId = value;

		Service::BlockServices::CreateBlock(gameObject);
		counter++;
	}

	/*glm::vec2 position = { 50, 50 };

	glm::vec2 scale = { 20, 20 };

	std::string text = ":) YAY... It worked!";

	for (int i = 0; i < std::size(text); i++)
	{
		GameObject gameObject{};
		gameObject.position = position + glm::vec2(i * scale.x, 0);
		gameObject.rotation = 0;
		gameObject.scale = scale;
		gameObject.color = { 0.0f, 0.0f, 0.0f };

		if (text[i] == ' ') continue;

		Service::GameObjectServices::CreateGameObject(gameObject.position, gameObject.rotation, gameObject.scale, gameObject.color, publicVulkanData->fontData.charsToGlyphIndex.at(text[i]));
	}*/


	//TextureInit::CreateTextureForFont();
	//TextureInit::CreateTextureForBezierLocation();
}

void ObjectController::ImGuiPlaceObject()
{
	
}

void ObjectController::Update()
{
	Service::ImGuiService::RunAllImGuiDocks();
	Service::EventResponseServices::TestAllEventServices();
}
