#pragma once
#include "Engine/ScriptAPI.h"
#include <string>

struct Script
{
	std::string className;
	IScript* instance = nullptr;
};