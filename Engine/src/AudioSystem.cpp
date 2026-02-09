#include "Engine/AudioSystem.h"

bool AudioSystem::Init() {

	FMOD_RESULT result;

	result = FMOD::Studio::System::create(&mStudio);
	/////
	/*
	Ici, permet de savoir si on a une erreur de type carte son not found, DLL not found, x86/x64 not good, FMOD not init
	*/
	/////
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	result = mStudio->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr);
	if (result != FMOD_OK) {
		std::cout << FMOD_ErrorString(result) << std::endl;
		return false;
	}

	mStudio->getCoreSystem(&mCore);
	return true;
}

void AudioSystem::LoadBanks() {
	FMOD::Studio::Bank* master = nullptr;
	FMOD::Studio::Bank* strings = nullptr;
	FMOD::Studio::Bank* musics = nullptr;

	// Ici, on lit les banks du project build dans la save de notre FMOD project.

	FMOD_RESULT result1 = mStudio->loadBankFile("E:/GitHub Desktop/Repositories/moteur-3d-nengine/Engine/Audio/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &master);
	if (result1 != FMOD_OK) std::cout << FMOD_ErrorString(result1) << std::endl;

	FMOD_RESULT result2 = mStudio->loadBankFile("E:/GitHub Desktop/Repositories/moteur-3d-nengine/Engine/Audio/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &strings);
	if (result2 != FMOD_OK) std::cout << FMOD_ErrorString(result2) << std::endl;

	FMOD_RESULT result3 = mStudio->loadBankFile("E:/GitHub Desktop/Repositories/moteur-3d-nengine/Engine/Audio/Musics.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &musics);
	if (result3 != FMOD_OK) std::cout << FMOD_ErrorString(result3) << std::endl;
}

void AudioSystem::PlayEvent(const char* path) {
	FMOD::Studio::EventDescription* desc = nullptr;
	FMOD::Studio::EventInstance* instance = nullptr;

	FMOD_RESULT result = mStudio->getEvent(path, &desc);
	if (result != FMOD_OK) { std::cout << FMOD_ErrorString(result) << std::endl; return; }
	mStudio->getEvent(path, &desc);

	result = desc->createInstance(&instance);
	if (result != FMOD_OK) { std::cout << FMOD_ErrorString(result) << std::endl; return; }
	instance->start();
	//instance->release();
}

void AudioSystem::Update() {
	mStudio->update();
}

void AudioSystem::Shutdown() {
	if (mStudio) {
		mStudio->unloadAll();
		mStudio->release();
		mStudio = nullptr;
	}
}