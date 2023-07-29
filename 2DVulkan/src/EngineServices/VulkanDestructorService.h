#pragma once
#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"
#include "EngineData/ObjectData/VulkanData.h"

class VulkanDestructorService
{

public:
	static void CleanUpSwapChain();
	static void CleanUpFrameBuffers();
	static void DeleteObject(VulkanData vulkanData);
};

