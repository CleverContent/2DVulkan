#pragma once
#include <map>
#include <string>

static enum BlockTypes
{
	EVENT,
	ACTION,
	CONDITION,
	UNKNOWNBLOCKTYPE,
};

static enum EventTypes
{
	OnClicks,
	UNKNOWNEVENTTYPE,
};

static enum ActionTypes
{
	CreateElement,
	Ignite,
	AddForce,
	UNKNOWNACTIONTYPE,
};

static enum ConditionTypes
{
	OnClick,
	UNKNOWNCONDITIONTYPE,
};

static std::map<EventTypes, std::string> EventTypesStrings =
{ 
	{EventTypes::OnClicks, "OnClick"},
	{EventTypes::UNKNOWNEVENTTYPE, "UNKNOWN"},
};

static std::map<ActionTypes, std::string> ActionTypesStrings =
{
	{ActionTypes::CreateElement, "CreateElement"},
	{ActionTypes::Ignite, "Ignite"},
	{ActionTypes::AddForce, "AddForce"},
	{ActionTypes::UNKNOWNACTIONTYPE, "UNKNOWN"},
};

static std::map<ConditionTypes, std::string> ConditionTypesStrings =
{
	{ConditionTypes::OnClick, "OnClick"},
	{ConditionTypes::UNKNOWNCONDITIONTYPE, "UNKNOWN"},
};