#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <stb_image.h>
#include <vector>
#include <glm.hpp>


struct Texture
{
	int texWidth = -1;
	int TexHeight = -1;
	int texChannels = -1;

	void* Image = nullptr;

	VkDeviceSize imageSize;

	VkBuffer staggingBuffer = VK_NULL_HANDLE;
	VkDeviceMemory staggingBufferMemory = VK_NULL_HANDLE;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	VkImageView textureImageView;
	VkSampler textureSampler;
};