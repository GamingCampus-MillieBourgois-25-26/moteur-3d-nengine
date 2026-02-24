#pragma once

extern "C" {

// Signatures exportées par la DLL de gameplay
__declspec(dllexport) bool GameModule_Init();
__declspec(dllexport) void GameModule_Update(float dt);
__declspec(dllexport) void GameModule_Shutdown();

}