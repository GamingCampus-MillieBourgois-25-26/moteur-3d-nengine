#pragma once
#include <Engine/ScriptAPI.h>
#include <iostream>

class PlayerController : public IScript {
public:
    void OnCreate() override {
        std::cout << "PlayerController created\n";
    }

    void OnUpdate(const ScriptContext& ctx) override {
        std::cout << "PlayerController update dt=" << ctx.dt << "\n";
    }

    void OnDestroy() override {
        std::cout << "PlayerController destroyed\n";
    }
};
