#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>

#include "PhysicalDeviceProperties.h"
#include "EngineData/Rectangle.h"
#include "EngineData/RenderData/VulkanData/UniformConstantsData.h"
#include "ImGuiData.h"
#include "Texture.h"

struct VulkanImageData
{
	VkSwapchainKHR swapChain = VK_NULL_HANDLE;

	std::vector<VkImage> swapChainImages{};
	std::vector<VkImageView> imageViews{};

	VkImageView depthImageView = VK_NULL_HANDLE;
	VkImage depthImage = VK_NULL_HANDLE;
	VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;

	bool windowResized = false;
};

struct CoreVulkanData
{
private:
	static CoreVulkanData* p_instance;

public:
	CoreVulkanData(const CoreVulkanData& obj) = delete;

	static CoreVulkanData* getInstance()
	{
		if (p_instance == nullptr)
		{
			p_instance = new CoreVulkanData{};
		}
		return p_instance;
	}

	VkInstance instance = VK_NULL_HANDLE;

	VkPhysicalDevice physicalDevice= VK_NULL_HANDLE;

	PhysicalDeviceProperties PhysicalDeviceProperties{};

	VkDevice device = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;
	VkQueue graphicsQueue = VK_NULL_HANDLE;

	VkCommandPool commandPool = VK_NULL_HANDLE;

	std::vector<VkCommandBuffer> commandBuffers{};
	std::vector<VkFramebuffer> frameBuffers{};

	VulkanImageData imageData{};

	VkRenderPass renderPass = VK_NULL_HANDLE;

	VkSurfaceKHR windowSurface = VK_NULL_HANDLE;

	std::vector<VkSemaphore> imageAvailabelSemaphores{};
	std::vector<VkSemaphore> renderFinishedSemaphores{};
	std::vector<VkFence> inFlightFences{};

	VkBuffer orthogonalMatrixBuffer{};
	VkDeviceMemory orthogonalMatrixBufferMemory{};
	void* orthogonalMatrixBufferMemoryMapped{};

	GLFWwindow* glfwWindow = NULL;
	glm::vec2 windowSize = { -1, -1 };

	ImGuiData imGUiData{};

	UniformConstantsData uniformConstantsData{};

	int currentFrameAheadCount = 0;
	int MaxFramesInFlight = 2;
};
