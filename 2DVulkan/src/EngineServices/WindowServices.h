#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"


#include <glm.hpp>
#include <stdexcept>
#include <array>

#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"
#include "EngineData/ObjectData/GameObject.h"

#include "EngineServices/VulkanInitializerService.h"

namespace Service
{
	class WindowServices
	{
	public:
		static bool HasWindowClosed(GLFWwindow* window);
		static glm::vec2 GetWindowSize(GLFWwindow* window);
		static glm::vec2 GetWindowLocation(GLFWwindow* window);

		static void SetCursorPositition();
		static glm::vec2 GetCursorPosition();
		static void HideCursor();
		static void SetCursor();

		static void StartImGuiFrame();

		static uint32_t ImGuiRender();

		static void VulkanRender(uint32_t imageIndex);
		static void RecordCommandBuffer( uint32_t imageIndex);

	};
}