#include "RenderController.h"


CoreVulkanData* CoreVulkanData::p_instance = nullptr;
PublicVulkanData* PublicVulkanData::instance = nullptr;

RenderController::RenderController(RenderControllerFlags rcf, std::map<Windows, WorldObjectsData>* objectsData, KeyInputData* eventData)
{
    this->eventData = eventData;
    worldObjectsData = objectsData;

    coreVulkanData = CoreVulkanData::getInstance();
    publicVulkanData = PublicVulkanData::getInstance();

    coreVulkanData->windowSize = rcf.windowSize;
    Service::VulkanInitializerService::PopulateCoreVulkanData();
    Service::VulkanInitializerService::PopulatePublicVulkanData();
    publicVulkanData->worldObjectsData = objectsData;
}

RenderController::~RenderController()
{
    //delete worldObjectsData;
}

void RenderController::StartUpdate()
{
    Service::WindowServices::StartImGuiFrame();
}

void RenderController::FinishUpdate()
{
    Render();
}

void RenderController::Render()
{
    bool renderingWithImGUI = false;
    uint32_t swapchainImageIndex = Service::WindowServices::ImGuiRender();
    Service::WindowServices::VulkanRender(swapchainImageIndex);
    coreVulkanData->currentFrameAheadCount = ((coreVulkanData->currentFrameAheadCount++) % coreVulkanData->MaxFramesInFlight);
}

bool RenderController::HasWindowClosed()
{
    return Service::WindowServices::HasWindowClosed(coreVulkanData->glfwWindow);
}