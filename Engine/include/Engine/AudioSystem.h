#pragma once
#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
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