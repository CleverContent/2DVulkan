#include "WindowServices.h"

namespace Service
{
	bool WindowServices::HasWindowClosed(GLFWwindow* window)
	{
		return !glfwWindowShouldClose(window);
	}

	glm::vec2 WindowServices::GetWindowSize(GLFWwindow* window)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		return glm::vec2{ width, height };
	}

	glm::vec2 WindowServices::GetWindowLocation(GLFWwindow* window)
	{
		return glm::vec2{-1, -1};
	}


	void WindowServices::SetCursorPositition()
	{

	}

	glm::vec2 WindowServices::GetCursorPosition()
	{
		return glm::vec2{-1, -1};
	}

	void WindowServices::HideCursor()
	{
	}

	void WindowServices::SetCursor()
	{

	}

	void WindowServices::StartImGuiFrame()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	uint32_t WindowServices::ImGuiRender()
	{
		CoreVulkanData& vulkanData = *CoreVulkanData::getInstance();
		
		VkDevice device = vulkanData.device;
		uint32_t currentFrame = vulkanData.currentFrameAheadCount;
		VkCommandBuffer commandBuffer = vulkanData.imGUiData.commandBuffers[currentFrame];

		vkWaitForFences(device, 1, &vulkanData.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = -1;

		VkResult result = vkAcquireNextImageKHR
		(
			vulkanData.device,
			vulkanData.imageData.swapChain,
			UINT64_MAX,
			vulkanData.imageAvailabelSemaphores[currentFrame],
			VK_NULL_HANDLE,
			&imageIndex
		);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {

			VulkanInitializerService::RecreateFrameBuffers();
			return -1;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		vkResetCommandBuffer(commandBuffer, 0);

		ImGui::Render();


		VkCommandBufferBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		vkBeginCommandBuffer(commandBuffer, &info);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanData.imGUiData.renderPass;
		renderPassInfo.framebuffer = vulkanData.imGUiData.frameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D{static_cast<uint32_t>(vulkanData.windowSize.x), static_cast<uint32_t>(vulkanData.windowSize.y)};

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		ImDrawData* drawData = ImGui::GetDrawData();

		ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		vkCmdEndRenderPass(commandBuffer);
		vkEndCommandBuffer(commandBuffer);

		return imageIndex;
	}

	void WindowServices::VulkanRender(uint32_t imageIndex)
	{
		CoreVulkanData& vulkanData = *CoreVulkanData::getInstance();

		VkDevice device = vulkanData.device;
		uint32_t currentFrame = vulkanData.currentFrameAheadCount;
		VkCommandBuffer commandBuffer = vulkanData.commandBuffers[currentFrame];

		vkWaitForFences(device, 1, &vulkanData.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		vkResetFences(device, 1, &vulkanData.inFlightFences[currentFrame] );

		vkResetCommandBuffer(commandBuffer, 0);

		RecordCommandBuffer(imageIndex);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { vulkanData.imageAvailabelSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		std::vector< VkCommandBuffer> submitCommandBuffers;

		submitCommandBuffers.resize(2);
		submitCommandBuffers[0] = commandBuffer;
		submitCommandBuffers[1] = vulkanData.imGUiData.commandBuffers[currentFrame];

		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		VkSemaphore signalSemaphores[] = { vulkanData.renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(vulkanData.graphicsQueue, 1, &submitInfo, vulkanData.inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkSwapchainKHR swapChains[] = { vulkanData.imageData.swapChain };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		VkResult result = vkQueuePresentKHR(vulkanData.presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vulkanData.imageData.windowResized) {
			vulkanData.imageData.windowResized = false;
			VulkanInitializerService::RecreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void WindowServices::RecordCommandBuffer(uint32_t imageIndex)
	{
		CoreVulkanData& vulkanData = *CoreVulkanData::getInstance();

		VkDevice device = vulkanData.device;
		uint32_t currentFrame = vulkanData.currentFrameAheadCount;
		VkCommandBuffer commandBuffer = vulkanData.commandBuffers[currentFrame];

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanData.renderPass;
		renderPassInfo.framebuffer = vulkanData.frameBuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = VkExtent2D{static_cast<uint32_t>(vulkanData.windowSize.x), static_cast<uint32_t>(vulkanData.windowSize.y)};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vulkanData.windowSize.x);
		viewport.height = static_cast<float>(vulkanData.windowSize.y);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D{ static_cast<uint32_t>(vulkanData.windowSize.x), static_cast<uint32_t>(vulkanData.windowSize.y) };
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		PublicVulkanData& publicVulkanData = *PublicVulkanData::getInstance();

		for (int i = (publicVulkanData.worldObjectsData->at(publicVulkanData.currentActiveWindow).gameObjects.size()) - 1; i >= 0; i--)
		{
			GameObject gameObject = publicVulkanData.worldObjectsData->at(publicVulkanData.currentActiveWindow).gameObjects[i];
			VkBuffer vertexBuffers[] = { gameObject.vulkanData.vertexBuffer };
			VkDeviceSize offsets[] = { 0 };

			VulkanInitializerService::UpdateUniformBuffers(gameObject, currentFrame);

			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gameObject.vulkanData.vulkanGraphicsPipeline);

			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffer, gameObject.vulkanData.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gameObject.vulkanData.vulkanPipelineLayout, 0, 1, &gameObject.vulkanData.vulkanDescriptorSets[currentFrame], 0, nullptr);

			vkCmdDrawIndexed(commandBuffer, gameObject.vulkanData.indexCount, 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}