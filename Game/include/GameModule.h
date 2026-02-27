#pragma once
#include "Engine/ScriptAPI.h"

extern "C" {

// Signatures exportées par la DLL de gameplay
	__declspec(dllexport)
	IScript* CreateScript(const char* name);

	__declspec(dllexport)
	void DestroyScript(IScript* script);


}