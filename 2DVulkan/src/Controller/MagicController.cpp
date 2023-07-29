#include "MagicController.h"

MagicController::MagicController(MagicControllerFlags MCF)
{

}

MagicController::~MagicController()
{

}

void MagicController::Init()
{

}

void MagicController::Update()
{

	if (PublicVulkanData::getInstance()->currentActiveWindow == SPELLCREATOR)
	{
		SpellCreatorImGui();
	}
}

void MagicController::SpellCreatorImGui()
{
	std::vector<std::string> blockNames = getSpellBlockNames();

	ImGui::Begin("SPELL CREATOR");

	for (int i = 0; i < blockNames.size(); i++)
	{
		if (ImGui::Button(blockNames.at(i).c_str()))
		{

		}
	}

	if (ImGui::Button("PlaceBlock"))
	{
		if(spellCreatorData.currentSelectedBlock != )
		spellCreatorData.placeBlock = true;
	}

	ImGui::End();
	/*ImGuiStyle* style = &ImGui::GetStyle();
	style->ItemSpacing = ImVec2(1.0f, 5.0f);*/
	

	//ImGui::Begin("Spell Creator");

	//if (ImGui::BeginTable("##table1", 3, ImGuiTableFlags_Borders))
	//{
	//	for (int i = 0; i < Spell::BuildingBlockCategories::Last_Category; i++)
	//	{
	//		std::string header = Spell::BuildingBlockCategoriesToString.at((Spell::BuildingBlockCategories)i);



	//		ImGui::TableNextColumn();

	//		float columnWidth = ImGui::GetColumnWidth();
	//		float textWidth = ImGui::CalcTextSize(header.c_str()).x;
	//		ImGui::SetCursorPosX((columnWidth - textWidth) * 0.5f + (columnWidth * i));
	//		ImGui::TableHeader(header.c_str());

	//		std::vector<std::string> blocks = {};

	//		switch ((Spell::BuildingBlockCategories)i)
	//		{
	//		case(Spell::Events):
	//			for (auto const [key, value] : Spell::EventBuildingBlocksToString)
	//			{
	//				blocks.push_back(value);
	//			}
	//			break;
	//		case(Spell::Actions):
	//			for (auto const [key, value] : Spell::ActionBuildingBlocksToString)
	//			{
	//				blocks.push_back(value);
	//			}
	//			break;
	//		case(Spell::Conditions):
	//			for (auto const [key, value] : Spell::ConditionBuildingBlocksToString)
	//			{
	//				blocks.push_back(value);
	//			}
	//			break;
	//		default:
	//			break;
	//		}

	//		for (auto const s : blocks)
	//		{
	//			if (ImGui::Button(s.c_str()))
	//			{
	//				std::cout << s << std::endl;
	//			}
	//		}

	//	}

	//	ImGui::EndTable();
	//}

	//ImGui::End();
}
