#pragma once
#include <glm.hpp>

struct Interface_OnClick
{
	virtual void OnClickCallBack(glm::vec2 mousePos) = 0;
};