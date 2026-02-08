#pragma once
#include "FMOD/FMOD Studio API Windows/api/core/inc/fmod.hpp"
#include "FMOD/FMOD Studio API Windows/api/studio/inc/fmod_studio.hpp"
#include "FMOD/FMOD Studio API Windows/api/core/inc/fmod_errors.h"
#include <iostream>

class AudioSystem {

public:

	bool Init();
	void LoadBanks();
	void Update();
	void Shutdown();

	void PlayEvent(const char* path);

private:

	FMOD::Studio::System* mStudio = nullptr;
	FMOD::System* mCore = nullptr;
};