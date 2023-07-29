#include "VulkanDestructorService.h"

void VulkanDestructorService::CleanUpSwapChain()
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();

	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}
	int width = 0, height = 0;
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(coreVulkanData->glfwWindow, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(coreVulkanData->device);

	VkDevice device = coreVulkanData->device;
	vkDestroyImageView(device, coreVulkanData->imageData.depthImageView, nullptr);
	vkDestroyImage(device, coreVulkanData->imageData.depthImage, nullptr);
	vkFreeMemory(device, coreVulkanData->imageData.depthImageMemory, nullptr);

	for (auto framebuffer : coreVulkanData->frameBuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	for (auto imageView : coreVulkanData->imageData.imageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, coreVulkanData->imageData.swapChain, nullptr);
}

void VulkanDestructorService::CleanUpFrameBuffers()
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}
	vkDeviceWaitIdle(coreVulkanData->device);

	for (int i = 0; i < coreVulkanData->frameBuffers.size(); i++)
		vkDestroyFramebuffer(coreVulkanData->device, coreVulkanData->frameBuffers[i], nullptr);

	for (int i = 0; i < coreVulkanData->imGUiData.frameBuffers.size(); i++)
		vkDestroyFramebuffer(coreVulkanData->device, coreVulkanData->imGUiData.frameBuffers[i], nullptr);
}

void VulkanDestructorService::DeleteObject(VulkanData vulkanData)
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}
	VkDevice device = coreVulkanData->device;

	vkDeviceWaitIdle(device);

	vkDestroyDescriptorPool(device, vulkanData.vulkanDescriptorPool, nullptr);

	vkDestroyDescriptorSetLayout(device, vulkanData.vulkanDescriptorSetLayout, nullptr);

	vkDestroyBuffer(device, vulkanData.vertexBuffer, nullptr);
	vkFreeMemory(device, vulkanData.vertexBufferMemory, nullptr);

	vkDestroyBuffer(device, vulkanData.indexBuffer, nullptr);
	vkFreeMemory(device, vulkanData.indexBufferMemory, nullptr);

	for (int i = 0; i < vulkanData.vulkanUniformBuffers.size(); i++)
	{
		vkDestroyBuffer(device, vulkanData.vulkanUniformBuffers[i], nullptr);
		vkFreeMemory(device, vulkanData.vulkanUniformMemory[i], nullptr);
	}

	vkDestroyPipeline(device, vulkanData.vulkanGraphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device, vulkanData.vulkanPipelineLayout, nullptr);

}
