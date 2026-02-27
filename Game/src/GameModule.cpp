#include "GameModule.h"
#include "PlayerController.h"
#include <Engine/ScriptAPI.h>
#include <iostream>

extern "C" {

    __declspec(dllexport)
        IScript* CreateScript(const char* name) {
        if (strcmp(name, "PlayerController") == 0)
            return new PlayerController();

        return nullptr;
    }

    __declspec(dllexport)
        void DestroyScript(IScript* script) {
        delete script;
    }

}
