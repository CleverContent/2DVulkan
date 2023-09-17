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
	glm::vec2 offset = { 20, 150 };






	/* This is almost what it should look like to make a object template, An object template is basically a different version of ECS
	* The only major difference between each object tmeplate is the shader and data being submitting to the shader. You can change absolutly
	* everything allowing for any type of object or shader.
	*/

	BindingLayout bindingLayout{};
	PushConstants pushConstants{};
	Vertex vertex{};

	bindingLayout.addDescriptorType({VK_SHADER_STAGE_VERTEX_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, 0});
	bindingLayout.addDescriptorType({VK_SHADER_STAGE_VERTEX_BIT, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, 0});  

	//Push Constants if any
	//
	//But none

	vertex.addVertexAttribute({VEC2, 0, 0});
	vertex.addVertexAttribute({VEC3, 1, 0});

	int BasicObjectId = Service::GameObjectService::CreateBlankObject(BasicObjectId, "Shader_BasicObject", bindingLayout, NULL, vertex);

	int objectId = Service::GameObjectService::CreateObjectInstance(BasicObjectId);








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
