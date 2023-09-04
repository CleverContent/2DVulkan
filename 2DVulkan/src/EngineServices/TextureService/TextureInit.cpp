#include "TextureInit.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::shared_ptr<Texture> TextureInit::CreateTextureForFont()
{
	CoreVulkanData& coreVulkanData = *CoreVulkanData::getInstance();

	std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture{});

	std::map<unsigned long, GlyphData> GlyphsData = FontManager::LoadFont();

	int maxSize = 0;

	float maxValue = 0;

	for (auto [key, value] : GlyphsData)
	{
		std::vector<glm::f32vec2>& points = value.points;

		if (points.size() > maxSize)
			maxSize = points.size();
		for (auto v : points)
		{
			if (v.x > maxValue)
				maxValue = v.x;
			if (v.y > maxValue)
				maxValue = v.y;
		}
	}

	texture->texWidth = maxSize;
	texture->TexHeight = GlyphsData.size();
	texture->texChannels = 2;

	int numberOfBytes = texture->texWidth * texture->TexHeight * texture->texChannels * sizeof(glm::f64);

	texture->Image = (void*)calloc(numberOfBytes, 1);

	texture->imageSize = numberOfBytes;


	int row = 0;

	for(auto [key, value] : GlyphsData)
	{
		float* pixelOffset = (float*)texture->Image + (row * (texture->texWidth * texture->texChannels));

		std::vector<glm::f32vec2>& points = value.points;

		int counter = 0;
		for (int i = 0; i < points.size(); i++)
		{

			pixelOffset[0] = ((float)points.at(i).x) / maxValue;
			pixelOffset[1] = ((float)points.at(i).y) / maxValue;

			pixelOffset += 2;

		}
		row++;
	}

	Service::VulkanInitializerService::CreateBuffer(texture->imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		texture->staggingBuffer, texture->staggingBufferMemory);

	void* data;
	vkMapMemory(coreVulkanData.device, texture->staggingBufferMemory, 0, texture->imageSize, 0, &data);
	memcpy(data, texture->Image, static_cast<size_t>(texture->imageSize));
	vkUnmapMemory(coreVulkanData.device, texture->staggingBufferMemory);

	//stbi_image_free(texture->Image);

	Service::VulkanInitializerService::CreateImage(texture->texWidth, texture->TexHeight, VK_FORMAT_R32G32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture->textureImage, texture->textureImageMemory);

	Service::VulkanInitializerService::TransitionImageLayout(texture->textureImage, VK_FORMAT_R32G32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Service::VulkanInitializerService::CopyBufferToImage(texture->staggingBuffer, texture->textureImage, static_cast<uint32_t>(texture->texWidth), static_cast<uint32_t>(texture->TexHeight));
	Service::VulkanInitializerService::TransitionImageLayout(texture->textureImage, VK_FORMAT_R32G32_SFLOAT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	Service::VulkanInitializerService::CreateTextureImageView(*texture, VK_FORMAT_R32G32_SFLOAT);
	Service::VulkanInitializerService::CreateTextureSampeler(*texture);

    return texture;
}

std::shared_ptr<GlyphTexture> TextureInit::CreateTextureForBezierLocation()
{
	CoreVulkanData& coreVulkanData = *CoreVulkanData::getInstance();

	std::shared_ptr<GlyphTexture> texture = std::make_shared<GlyphTexture>(GlyphTexture{});

	std::map<unsigned long, GlyphData> GlyphsData = FontManager::LoadFont();


	texture->texWidth = 1;
	texture->TexHeight = GlyphsData.size();
	texture->texChannels = 1;

	int size = texture->texWidth * texture->TexHeight * texture->texChannels * sizeof(glm::i8);

	texture->Image = (void*)calloc(size, 1);

	texture->imageSize = size;

	glm::u8* pixelOffset = (glm::u8*)texture->Image;

	int row = 0;

	int memoryUsed = 0;

	for (auto [glyphChar, glyphData] : GlyphsData)
	{
		pixelOffset = (glm::u8*)texture->Image + (row * texture->texWidth);

		std::vector<BezierStruct> BeizerGlyphData = CreateBezierLocationList(glyphData);

		glm::u8 beizerCount = BeizerGlyphData.size() + glyphData.numberOfContours - 1;

		pixelOffset[0] = beizerCount;
		row++;
	}

	Service::VulkanInitializerService::CreateBuffer(texture->imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		texture->staggingBuffer, texture->staggingBufferMemory);

	void* data;
	vkMapMemory(coreVulkanData.device, texture->staggingBufferMemory, 0, texture->imageSize, 0, &data);
	std::memcpy(data, texture->Image, static_cast<size_t>(texture->imageSize));
	vkUnmapMemory(coreVulkanData.device, texture->staggingBufferMemory);

	stbi_image_free(texture->Image);

	Service::VulkanInitializerService::CreateImage(texture->texWidth, texture->TexHeight, VK_FORMAT_R8_UINT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, texture->textureImage, texture->textureImageMemory);

	Service::VulkanInitializerService::TransitionImageLayout(texture->textureImage, VK_FORMAT_R8_UINT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	Service::VulkanInitializerService::CopyBufferToImage(texture->staggingBuffer, texture->textureImage, static_cast<uint32_t>(texture->texWidth), static_cast<uint32_t>(texture->TexHeight));
	Service::VulkanInitializerService::TransitionImageLayout(texture->textureImage, VK_FORMAT_R8_UINT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	Service::VulkanInitializerService::CreateTextureImageView(*texture, VK_FORMAT_R8_UINT);
	Service::VulkanInitializerService::CreateTextureSampeler(*texture);

	return texture;
}


std::vector<BezierStruct> TextureInit::CreateBezierLocationList(GlyphData data)
{
	std::vector<BezierCurve> bezierCurves = CreateBezierList(data);

	std::vector<BezierStruct> result(bezierCurves.size());

	int counter = 0;
	int bezierCounter = 0;
	for (BezierCurve b : bezierCurves)
	{
		BezierStruct bs{};

		bs.points = b;

		bs.yCoordInTexture = data.GlyphID;
		bs.xCoordInTexture = bezierCounter;


		result.at(counter) = bs;
		bezierCounter += 2;
		counter++;
	}
	return result;
}

std::vector<BezierCurve> TextureInit::CreateBezierList(GlyphData data)
{
	std::vector<BezierCurve> result{};


	for (int i = 0; i < (data.points.size() - 1); i += 2)
	{
		//if (data.tags.at(i) == true && data.tags.at(i + 1) == true)
		//{
		//	i -= 1;
		//	continue;
		//}
		BezierCurve bc{};

		bc.p1 = (glm::i16vec2)data.points.at(i);
		bc.p2 = (glm::i16vec2)data.points.at(i + 1);
		bc.p3 = (glm::i16vec2)data.points.at(i + 2);

		if (bc.p2.x < 0 && bc.p2.y < 0)
		{
			continue;
		}

		result.push_back(bc);
	}

	return result;
}

std::pair<glm::vec2, glm::vec2> TextureInit::GetMinAndMax(std::vector<glm::i16vec2> bezierCurve)
{
	glm::i32vec2 min = {INT_MAX, INT_MAX};
	glm::i32vec2 max = {0, 0};

	for (auto b : bezierCurve)
	{
		if (b.x < min.x)
			min.x = b.x;

		if (b.x > max.x)
			max.x = b.x;

		if (b.y < min.y)
			min.y = b.y;

		if (b.y > max.y)
			max.y = b.y;
	}

	return {min, max};
}
