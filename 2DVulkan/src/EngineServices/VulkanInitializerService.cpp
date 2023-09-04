#include "VulkanInitializerService.h"
#include <gtx/string_cast.hpp>
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE

namespace Service
{
	CoreVulkanData* VulkanInitializerService::coreVulkanData = nullptr;
	PublicVulkanData* VulkanInitializerService::publicVulkanData = nullptr;

	void VulkanInitializerService::PopulateCoreVulkanData()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();

		CreateInstance();
		CreatePhysicalDevice();
		CreateWindowSurface();
		CreateLogicalDevice();
		CreateSwapChain();
		CreateImageViews();

		StartSetupImGUI();
		CreateImGuiDescriptorPool();

		CreateRenderPass();
		CreateCommandPool();
		CreateCommandBuffers();
		CreateDepthResources();
		CreateFrameBuffers();
		CreateSyncObjects();
		CreateUniformConstants();

		FinishSetupImGui();
	}

	void VulkanInitializerService::PopulatePublicVulkanData()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();
		publicVulkanData->windowSize = coreVulkanData->windowSize;
	}

	void VulkanInitializerService::PopulateVulkanData(VulkanData& vulkanData)
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();

		CreateUniformBuffers(vulkanData);
		CreateDescriptorSetLayout(vulkanData);
		CreatePipelineLayout(vulkanData);
		CreateGraphicsPipeline(vulkanData);
		CreateDescriptorPool(vulkanData);
		CreateDescriptorSets(vulkanData);

	}

	void VulkanInitializerService::StartSetupImGUI()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
	}

	void VulkanInitializerService::FinishSetupImGui()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();
		ImGui_ImplGlfw_InitForVulkan(coreVulkanData->glfwWindow, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = coreVulkanData->instance;
		init_info.PhysicalDevice = coreVulkanData->physicalDevice;
		init_info.Device = coreVulkanData->device;
		init_info.QueueFamily = coreVulkanData->PhysicalDeviceProperties.graphicsIndex.value();
		init_info.Queue = coreVulkanData->graphicsQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = coreVulkanData->imGUiData.descriptorPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = coreVulkanData->MaxFramesInFlight;
		init_info.ImageCount = coreVulkanData->MaxFramesInFlight;
		ImGui_ImplVulkan_Init(&init_info, coreVulkanData->imGUiData.renderPass);

		VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

		ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

		EndSingleTimeCommands(commandBuffer);
	}

	void VulkanInitializerService::CreateImGuiDescriptorPool()
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		if (vkCreateDescriptorPool(coreVulkanData->device, &pool_info, NULL, &coreVulkanData->imGUiData.descriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to Create IMGUI Descriptor Pool!");
		}
	}

	void VulkanInitializerService::CreateInstance()
	{
		glfwInit();

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Clever_Engine";
		appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
		appInfo.pEngineName = "Clever";
		appInfo.apiVersion = VK_API_VERSION_1_0;

		auto extensions = GetRequiredExtensions();

		const std::vector<const char*> validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		PopulateDebugMessengerCreateInfo(debugCreateInfo);

		VkInstanceCreateInfo instanceInfo{};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = (uint32_t)extensions.size();
		instanceInfo.ppEnabledExtensionNames = extensions.data();
		instanceInfo.enabledLayerCount = (uint32_t)validationLayers.size();
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
		instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

		if (vkCreateInstance(&instanceInfo, nullptr, &coreVulkanData->instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}

		VkResult result;

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(coreVulkanData->instance, "vkCreateDebugUtilsMessengerEXT");

		VkDebugUtilsMessengerEXT vkDebugMesseneger;

		if (func != nullptr) {
			result = func(coreVulkanData->instance, &debugCreateInfo, nullptr, &vkDebugMesseneger);
		}
		else {
			result = VK_ERROR_EXTENSION_NOT_PRESENT;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
	}

	void VulkanInitializerService::CreateLogicalDevice()
	{
		std::vector <VkDeviceQueueCreateInfo> queueCreateInfo;

		uint32_t queueFamilyPropertyCount = 0;
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;

		vkGetPhysicalDeviceQueueFamilyProperties(coreVulkanData->physicalDevice, &queueFamilyPropertyCount, nullptr);

		queueFamilyProperties.resize(queueFamilyPropertyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(coreVulkanData->physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

		bool graphicsQueue = false;
		bool presentQueue = false;

		PhysicalDeviceProperties QFI = GetPhysicalDeviceProperties(coreVulkanData->physicalDevice);

		float queuePriority = 1.0f;
		std::set<uint32_t> uniqueIndicies = { QFI.graphicsIndex.value(), QFI.presentIndex.value() };
		for (auto index : uniqueIndicies)
		{
			VkDeviceQueueCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			info.queueFamilyIndex = index;
			info.queueCount = 1;
			info.pQueuePriorities = &queuePriority;
			queueCreateInfo.push_back(info);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = true;
		deviceFeatures.wideLines = true;

		std::vector<const char*> deviceExtextions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		std::vector<const char*> validationLayers =
		{
			"VK_LAYER_KHRONOS_validation"
		};

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfo.size());
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtextions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtextions.data();
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

		if (vkCreateDevice(coreVulkanData->physicalDevice, &deviceCreateInfo, nullptr, &coreVulkanData->device) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create Logical Device!");
		}

		vkGetDeviceQueue(coreVulkanData->device, coreVulkanData->PhysicalDeviceProperties.graphicsIndex.value(), 0, &coreVulkanData->graphicsQueue);
		vkGetDeviceQueue(coreVulkanData->device, coreVulkanData->PhysicalDeviceProperties.presentIndex.value(), 0, &coreVulkanData->presentQueue);
	}

	void VulkanInitializerService::CreatePhysicalDevice()
	{
		uint32_t physicalDeviceCount = 0;

		vkEnumeratePhysicalDevices(coreVulkanData->instance, &physicalDeviceCount, NULL);

		std::vector<VkPhysicalDevice> possiblePhysicalDevices;
		std::vector<VkPhysicalDeviceProperties> physicalDeviceProperties;

		possiblePhysicalDevices.resize(physicalDeviceCount);
		physicalDeviceProperties.resize(physicalDeviceCount);

		if (vkEnumeratePhysicalDevices(coreVulkanData->instance, &physicalDeviceCount, possiblePhysicalDevices.data()))
		{
			throw std::runtime_error("Unable to find able physical devices");
		}

		for (VkPhysicalDevice physicalDevice : possiblePhysicalDevices)
		{
			PhysicalDeviceProperties physicalDeviceData = GetPhysicalDeviceProperties(physicalDevice);

			if (physicalDeviceData.isComplete())
			{
				coreVulkanData->physicalDevice = physicalDevice;
				coreVulkanData->PhysicalDeviceProperties = physicalDeviceData;
				return;
			}
		}
		throw std::runtime_error("Failed to pick a Physical Device");
	}

	void VulkanInitializerService::CreateWindowSurface()
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		coreVulkanData->glfwWindow = glfwCreateWindow(coreVulkanData->windowSize.x, coreVulkanData->windowSize.y, "Clever", nullptr, nullptr);

		if (!coreVulkanData->glfwWindow)
		{
			throw std::runtime_error("failed to create GLFW window!");
		}
		if (glfwCreateWindowSurface(coreVulkanData->instance, coreVulkanData->glfwWindow, nullptr, &coreVulkanData->windowSurface))
		{
			throw std::runtime_error("Failed to create Window Surface");
		}
	}

	void VulkanInitializerService::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport;

		VkPhysicalDevice physicalDevice = coreVulkanData->physicalDevice;
		VkSurfaceKHR surface = coreVulkanData->windowSurface;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainSupport.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			swapChainSupport.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainSupport.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			swapChainSupport.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapChainSupport.presentModes.data());
		}

		coreVulkanData->windowSize.x = std::clamp(static_cast<uint32_t>(coreVulkanData->windowSize.x), swapChainSupport.capabilities.minImageExtent.width, swapChainSupport.capabilities.maxImageExtent.width);
		coreVulkanData->windowSize.y = std::clamp(static_cast<uint32_t>(coreVulkanData->windowSize.y), swapChainSupport.capabilities.minImageExtent.height, swapChainSupport.capabilities.maxImageExtent.height);

		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = swapChainSupport.capabilities.minImageCount + 1;
		swapchainCreateInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
		swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainCreateInfo.imageExtent = VkExtent2D{ static_cast<uint32_t>(coreVulkanData->windowSize.x), static_cast<uint32_t>(coreVulkanData->windowSize.y)} ;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		uint32_t queueFamilyIndices[] = { coreVulkanData->PhysicalDeviceProperties.graphicsIndex.value(), coreVulkanData->PhysicalDeviceProperties.presentIndex.value() };

		if (coreVulkanData->PhysicalDeviceProperties.graphicsIndex.value() != coreVulkanData->PhysicalDeviceProperties.presentIndex.value())
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(coreVulkanData->device, &swapchainCreateInfo, nullptr, &coreVulkanData->imageData.swapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create Swap Chain!");
		}
	}

	void VulkanInitializerService::CreateImageViews()
	{
		uint32_t swapChainImageCount;
		vkGetSwapchainImagesKHR(coreVulkanData->device, coreVulkanData->imageData.swapChain, &swapChainImageCount, nullptr);

		coreVulkanData->imageData.swapChainImages.resize(swapChainImageCount);
		coreVulkanData->imageData.imageViews.resize(swapChainImageCount);

		vkGetSwapchainImagesKHR(coreVulkanData->device, coreVulkanData->imageData.swapChain, &swapChainImageCount, coreVulkanData->imageData.swapChainImages.data());

		int i = 0;
		for (VkImage image : coreVulkanData->imageData.swapChainImages)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = image;
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = VK_FORMAT_B8G8R8A8_UNORM;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(coreVulkanData->device, &createInfo, nullptr, &coreVulkanData->imageData.imageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Unable to make Image View!");
			}
			i++;
		}
	}

	void VulkanInitializerService::CreateRenderPass()
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentReference{};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDesciption{};
		subpassDesciption.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesciption.colorAttachmentCount = 1;
		subpassDesciption.pColorAttachments = &attachmentReference;
		subpassDesciption.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		createInfo.pAttachments = attachments.data();
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpassDesciption;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(coreVulkanData->device, &createInfo, nullptr, &coreVulkanData->renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create RenderPass");
		}

		VkAttachmentDescription colorAttachmentImGUi{};
		colorAttachmentImGUi.format = VK_FORMAT_B8G8R8A8_UNORM;
		colorAttachmentImGUi.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentImGUi.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		colorAttachmentImGUi.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentImGUi.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentImGUi.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentImGUi.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentImGUi.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference attachmentReferenceImGUi{};
		attachmentReferenceImGUi.attachment = 0;
		attachmentReferenceImGUi.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDesciptionImGUi{};
		subpassDesciptionImGUi.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesciptionImGUi.colorAttachmentCount = 1;
		subpassDesciptionImGUi.pColorAttachments = &attachmentReferenceImGUi;

		VkSubpassDependency dependencyImGUi{};
		dependencyImGUi.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencyImGUi.dstSubpass = 0;
		dependencyImGUi.srcAccessMask = 0;
		dependencyImGUi.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencyImGUi.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencyImGUi.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 1> attachmentsImGUI = { colorAttachmentImGUi };

		VkRenderPassCreateInfo renderPassInfoImGUI{};
		renderPassInfoImGUI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfoImGUI.attachmentCount = static_cast<uint32_t>(attachmentsImGUI.size());
		renderPassInfoImGUI.pAttachments = attachmentsImGUI.data();
		renderPassInfoImGUI.subpassCount = 1;
		renderPassInfoImGUI.pSubpasses = &subpassDesciptionImGUi;
		renderPassInfoImGUI.dependencyCount = 1;
		renderPassInfoImGUI.pDependencies = &dependencyImGUi;

		VkRenderPass renderPass{};

		if (vkCreateRenderPass(coreVulkanData->device, &renderPassInfoImGUI, nullptr, &coreVulkanData->imGUiData.renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create ImGui RenderPass!");
		}

	}

	void VulkanInitializerService::CreateCommandPool()
	{
		VkCommandPoolCreateInfo commandPoolInfo{};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolInfo.queueFamilyIndex = coreVulkanData->PhysicalDeviceProperties.graphicsIndex.value();

		if (vkCreateCommandPool(coreVulkanData->device, &commandPoolInfo, nullptr, &coreVulkanData->commandPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create Command Pool!");

		if (vkCreateCommandPool(coreVulkanData->device, &commandPoolInfo, nullptr, &coreVulkanData->imGUiData.commandPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create Command Pool!");
	}

	void VulkanInitializerService::CreateCommandBuffers()
	{
		coreVulkanData->commandBuffers.resize(coreVulkanData->MaxFramesInFlight);
		coreVulkanData->imGUiData.commandBuffers.resize(coreVulkanData->MaxFramesInFlight);

		VkCommandBufferAllocateInfo allocationInfo{};
		allocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocationInfo.commandPool = coreVulkanData->commandPool;
		allocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocationInfo.commandBufferCount = static_cast<uint32_t>(coreVulkanData->commandBuffers.size());

		if (vkAllocateCommandBuffers(coreVulkanData->device, &allocationInfo, coreVulkanData->commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allcocate Command Buffers");
		}

		allocationInfo.commandPool = coreVulkanData->imGUiData.commandPool;

		if (vkAllocateCommandBuffers(coreVulkanData->device, &allocationInfo, coreVulkanData->imGUiData.commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Unable to allcocate Command Buffers");
		}

	}

	void VulkanInitializerService::CreateDepthResources()
	{
		VkFormat depthFormat = FindDepthFormat();

		CreateImage(coreVulkanData->windowSize.x, coreVulkanData->windowSize.y, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, coreVulkanData->imageData.depthImage, coreVulkanData->imageData.depthImageMemory);
		coreVulkanData->imageData.depthImageView = CreateImageView(coreVulkanData->imageData.depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	void VulkanInitializerService::CreateFrameBuffers()
	{
		int i = 0;
		coreVulkanData->frameBuffers.resize(coreVulkanData->imageData.imageViews.size());
		coreVulkanData->imGUiData.frameBuffers.resize(coreVulkanData->imageData.imageViews.size());

		for (VkImageView imageView : coreVulkanData->imageData.imageViews)
		{
			std::vector<VkImageView> attachments;
			if (coreVulkanData->imageData.depthImageView != VK_NULL_HANDLE)
			{
				attachments.push_back(imageView);
				attachments.push_back(coreVulkanData->imageData.depthImageView);
			}
			else
			{
				attachments.push_back(imageView);
			}

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = coreVulkanData->renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = coreVulkanData->windowSize.x;
			framebufferInfo.height = coreVulkanData->windowSize.y;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(coreVulkanData->device, &framebufferInfo, nullptr, &coreVulkanData->frameBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Could not create Framebuffer!");
			}

			attachments.clear();
			attachments = { imageView };

			framebufferInfo.renderPass = coreVulkanData->imGUiData.renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();

			if (vkCreateFramebuffer(coreVulkanData->device, &framebufferInfo, nullptr, &coreVulkanData->imGUiData.frameBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Could not create Framebuffer!");
			}

			i++;
		}
	}

	void VulkanInitializerService::CreateSyncObjects()
	{
		coreVulkanData->imageAvailabelSemaphores.resize(coreVulkanData->MaxFramesInFlight);
		coreVulkanData->renderFinishedSemaphores.resize(coreVulkanData->MaxFramesInFlight);
		coreVulkanData->inFlightFences.resize(coreVulkanData->MaxFramesInFlight);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		for (size_t i = 0; i < coreVulkanData->MaxFramesInFlight; i++)
		{
			if (vkCreateSemaphore(coreVulkanData->device, &semaphoreInfo, nullptr, &coreVulkanData->imageAvailabelSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(coreVulkanData->device, &semaphoreInfo, nullptr, &coreVulkanData->renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(coreVulkanData->device, &fenceInfo, nullptr, &coreVulkanData->inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create semaphores!");
			}
		}
	}

	void VulkanInitializerService::CreateUniformConstants()
	{
		VkDeviceSize bufferSize = sizeof(UniformConstantsData);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, coreVulkanData->orthogonalMatrixBuffer, coreVulkanData->orthogonalMatrixBufferMemory);

		coreVulkanData->uniformConstantsData.orthogonalMatrix = glm::orthoZO(0.0f, coreVulkanData->windowSize.x, 0.0f, coreVulkanData->windowSize.y, 0.0f, -2.0f);

		vkMapMemory(coreVulkanData->device, coreVulkanData->orthogonalMatrixBufferMemory, 0, bufferSize, 0, &coreVulkanData->orthogonalMatrixBufferMemoryMapped);

		memcpy(coreVulkanData->orthogonalMatrixBufferMemoryMapped, &coreVulkanData->uniformConstantsData, sizeof(coreVulkanData->uniformConstantsData));

		//std::cout << "OrthogonalMatrix:   " << glm::to_string(vulkanData.uniformConstantsData.orthogonalMatrix) << std::endl;

	}

	void VulkanInitializerService::RecreateSwapChain()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();
		VulkanDestructorService::CleanUpSwapChain();

		CreateSwapChain();
		CreateImageViews();
		CreateDepthResources();
		CreateFrameBuffers();
	}

	void VulkanInitializerService::RecreateFrameBuffers()
	{
		if (coreVulkanData == nullptr)
			coreVulkanData = CoreVulkanData::getInstance();
		if (publicVulkanData == nullptr)
			publicVulkanData = PublicVulkanData::getInstance();
		VulkanDestructorService::CleanUpFrameBuffers();

		CreateFrameBuffers();
	}

	void VulkanInitializerService::CreateUniformBuffers(VulkanData& vulkanData)
	{
		VkDeviceSize bufferSize = sizeof(ModelMatrix);

		vulkanData.vulkanUniformBuffers.resize(coreVulkanData->MaxFramesInFlight);
		vulkanData.vulkanUniformMemory.resize(coreVulkanData->MaxFramesInFlight);
		vulkanData.vulkanUniformBuffersMapped.resize(coreVulkanData->MaxFramesInFlight);

		for (size_t i = 0; i < coreVulkanData->MaxFramesInFlight; i++) {
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vulkanData.vulkanUniformBuffers[i], vulkanData.vulkanUniformMemory[i]);

			vkMapMemory(coreVulkanData->device, vulkanData.vulkanUniformMemory[i], 0, bufferSize, 0, &vulkanData.vulkanUniformBuffersMapped[i]);
		}
	}

	void VulkanInitializerService::CreateDescriptorSetLayout(VulkanData& vulkanData)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindingInfos(2);
		bindingInfos[0].binding = 0;
		bindingInfos[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bindingInfos[0].descriptorCount = 1;
		bindingInfos[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		bindingInfos[1].binding = 1;
		bindingInfos[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bindingInfos[1].descriptorCount = 1;
		bindingInfos[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		if (vulkanData.texture->Image != nullptr && vulkanData.glyphTexture->Image != nullptr)
		{
			bindingInfos.resize(4);

			bindingInfos[2].binding = 2;
			bindingInfos[2].descriptorCount = 1;
			bindingInfos[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bindingInfos[2].pImmutableSamplers = nullptr;
			bindingInfos[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			bindingInfos[3].binding = 3;
			bindingInfos[3].descriptorCount = 1;
			bindingInfos[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bindingInfos[3].pImmutableSamplers = nullptr;
			bindingInfos[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		VkDescriptorSetLayoutCreateInfo desciptorInfo{};
		desciptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		desciptorInfo.bindingCount = static_cast<uint32_t>(bindingInfos.size());
		desciptorInfo.pBindings = bindingInfos.data();


		if (vkCreateDescriptorSetLayout(coreVulkanData->device, &desciptorInfo, nullptr, &vulkanData.vulkanDescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("DesctiptorSetLayout failed to be created!");
		}
	}

	void VulkanInitializerService::CreatePipelineLayout(VulkanData& vulkanData)
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(FontPushConstants);
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;


		VkPipelineLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		info.setLayoutCount = 1;
		info.pSetLayouts = &vulkanData.vulkanDescriptorSetLayout;
		info.pushConstantRangeCount = 1;
		info.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(coreVulkanData->device, &info, nullptr, &vulkanData.vulkanPipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Pipeline Layout failed to be created!");
		}
	}

	void VulkanInitializerService::CreateGraphicsPipeline(VulkanData& vulkanData)
	{
		auto vertShaderCode = ReadShaderFile("src/Resources/Shaders/TextVert.spv");
		auto fragShaderCode = ReadShaderFile("src/Resources/Shaders/TextFrag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStageCreateInfo[] = { vertShaderStageInfo, fragShaderStageInfo };

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;

		//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = 2;
		createInfo.pStages = shaderStageCreateInfo;
		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &inputAssembly;
		createInfo.pViewportState = &viewportState;
		createInfo.pRasterizationState = &rasterizer;
		createInfo.pMultisampleState = &multisampling;
		createInfo.pColorBlendState = &colorBlending;
		createInfo.pDynamicState = &dynamicState;
		createInfo.pDepthStencilState = &depthStencil;
		createInfo.layout = vulkanData.vulkanPipelineLayout;
		createInfo.renderPass = coreVulkanData->renderPass;
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(coreVulkanData->device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &vulkanData.vulkanGraphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Graphics Pipeline");
		}

		vkDestroyShaderModule(coreVulkanData->device, fragShaderModule, nullptr);
		vkDestroyShaderModule(coreVulkanData->device, vertShaderModule, nullptr);
	}

	void VulkanInitializerService::CreateDescriptorPool(VulkanData& vulkanData)
	{
		std::vector<VkDescriptorPoolSize> sizeInfo(1);
		sizeInfo[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		sizeInfo[0].descriptorCount = static_cast<uint32_t>(coreVulkanData->MaxFramesInFlight);

		if (vulkanData.texture->Image != nullptr)
		{
			sizeInfo.resize(2);
			sizeInfo[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			sizeInfo[1].descriptorCount = static_cast<uint32_t>(coreVulkanData->MaxFramesInFlight);
		}

		VkDescriptorPoolCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		info.poolSizeCount = static_cast<uint32_t>(sizeInfo.size());
		info.pPoolSizes = sizeInfo.data();
		info.maxSets = static_cast<uint32_t>(coreVulkanData->MaxFramesInFlight);

		if (vkCreateDescriptorPool(coreVulkanData->device, &info, nullptr, &vulkanData.vulkanDescriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool");
		}
	}

	void VulkanInitializerService::CreateDescriptorSets(VulkanData& vulkanData)
	{
		std::vector<VkDescriptorSetLayout> layouts(coreVulkanData->MaxFramesInFlight, vulkanData.vulkanDescriptorSetLayout);
		VkDescriptorSetAllocateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		info.descriptorPool = vulkanData.vulkanDescriptorPool;
		info.descriptorSetCount = static_cast<uint32_t>(coreVulkanData->MaxFramesInFlight);
		info.pSetLayouts = layouts.data();

		vulkanData.vulkanDescriptorSets.resize(coreVulkanData->MaxFramesInFlight);

		if (vkAllocateDescriptorSets(coreVulkanData->device, &info, vulkanData.vulkanDescriptorSets.data()) != VK_SUCCESS)
		{
			std::runtime_error("Falied to allocate Descriptor sets");
		}

		for (size_t i = 0; i < coreVulkanData->MaxFramesInFlight; i++) {

			std::array<VkDescriptorBufferInfo, 2> bufferInfos;
			bufferInfos[0].buffer = vulkanData.vulkanUniformBuffers[i];
			bufferInfos[0].offset = 0;
			bufferInfos[0].range = sizeof(ModelMatrix);

			bufferInfos[1].buffer = coreVulkanData->orthogonalMatrixBuffer;
			bufferInfos[1].offset = 0;
			bufferInfos[1].range = sizeof(UniformConstantsData);

			std::vector<VkDescriptorImageInfo> imageInfo{};

			if (vulkanData.texture->Image != nullptr && vulkanData.glyphTexture->Image != nullptr)
			{
				imageInfo.resize(2);

				imageInfo[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo[0].imageView = vulkanData.texture->textureImageView;
				imageInfo[0].sampler = vulkanData.texture->textureSampler;

				imageInfo[1].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo[1].imageView = vulkanData.glyphTexture->textureImageView;
				imageInfo[1].sampler = vulkanData.glyphTexture->textureSampler;
			}
			
			std::vector<VkWriteDescriptorSet> writeInfo(1);
			writeInfo[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writeInfo[0].dstSet = vulkanData.vulkanDescriptorSets[i];
			writeInfo[0].dstBinding = 0;
			writeInfo[0].dstArrayElement = 0;
			writeInfo[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			writeInfo[0].descriptorCount = static_cast<uint32_t>(bufferInfos.size());
			writeInfo[0].pBufferInfo = bufferInfos.data();

			if (vulkanData.texture->Image != nullptr)
			{
				writeInfo.resize(3);

				writeInfo[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeInfo[1].dstSet = vulkanData.vulkanDescriptorSets[i];
				writeInfo[1].dstBinding = 2;
				writeInfo[1].dstArrayElement = 0;
				writeInfo[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				writeInfo[1].descriptorCount = 1;
				writeInfo[1].pImageInfo = &imageInfo.at(0);

				writeInfo[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeInfo[2].dstSet = vulkanData.vulkanDescriptorSets[i];
				writeInfo[2].dstBinding = 3;
				writeInfo[2].dstArrayElement = 0;
				writeInfo[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				writeInfo[2].descriptorCount = 1;
				writeInfo[2].pImageInfo = &imageInfo.at(1);
			}
			vkUpdateDescriptorSets(coreVulkanData->device, static_cast<uint32_t>(writeInfo.size()), writeInfo.data(), 0, nullptr);
		}
	}

	void VulkanInitializerService::CreateObjectBuffers(VulkanData& vulkanData, std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		vulkanData.indexCount = indices.size();

		{
			VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			CreateBuffer( bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(coreVulkanData->device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices.data(), (size_t)bufferSize);
			vkUnmapMemory(coreVulkanData->device, stagingBufferMemory);

			CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vulkanData.indexBuffer, vulkanData.indexBufferMemory);

			CopyBuffer(stagingBuffer, vulkanData.indexBuffer, bufferSize);

			vkDestroyBuffer(coreVulkanData->device, stagingBuffer, nullptr);
			vkFreeMemory(coreVulkanData->device, stagingBufferMemory, nullptr);
		}

		{
			VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			void* data;
			vkMapMemory(coreVulkanData->device, stagingBufferMemory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), (size_t)bufferSize);
			vkUnmapMemory(coreVulkanData->device, stagingBufferMemory);

			CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vulkanData.vertexBuffer, vulkanData.vertexBufferMemory);

			CopyBuffer(stagingBuffer, vulkanData.vertexBuffer, bufferSize);

			vkDestroyBuffer(coreVulkanData->device, stagingBuffer, nullptr);
			vkFreeMemory(coreVulkanData->device, stagingBufferMemory, nullptr);
		}
	}

	void VulkanInitializerService::UpdateUniformBuffers(GameObject& gameObject, uint32_t currentFrame)
	{
		ModelMatrix model{};
		model.modelMatrix = glm::translate(glm::mat4(1.0f), { gameObject.position.x, gameObject.position.y, 0.0f});
		model.modelMatrix = glm::rotate(model.modelMatrix, gameObject.rotation, glm::vec3(0, 0, 1));
		model.modelMatrix = glm::scale(model.modelMatrix, { gameObject.scale, 0.0f });

		memcpy(gameObject.vulkanData.vulkanUniformBuffersMapped[currentFrame], &model, sizeof(model));

		//std::cout << "ModelMatrix:   " << glm::to_string(model.modelMatrix) << std::endl;
	}

	PhysicalDeviceProperties VulkanInitializerService::GetPhysicalDeviceProperties(VkPhysicalDevice physicalDevice)
	{
		uint32_t queueFamilyPropertyCount = 0;
		std::vector<VkQueueFamilyProperties> queueFamilyPropteries;

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

		queueFamilyPropteries.resize(queueFamilyPropertyCount);

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyPropteries.data());

		VkPhysicalDeviceProperties physicalDeviceProperties;

		vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

		PhysicalDeviceProperties physicalDeviceData;

		physicalDeviceData.deviceName = physicalDeviceProperties.deviceName;

		int i = 0;
		for (const auto& queueFamilyProperty : queueFamilyPropteries)
		{
			if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				physicalDeviceData.graphicsIndex = i;
			}
			if (queueFamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
			{
				physicalDeviceData.presentIndex = i;
			}
			if (physicalDeviceData.graphicsIndex.has_value() && physicalDeviceData.presentIndex.has_value())
				break;

			i++;
		}
		return physicalDeviceData;
	}
}