#pragma once

struct ScriptContext {
    float dt;
};

class IScript {
public:
    virtual ~IScript() = default;
    virtual void OnCreate() {}
    virtual void OnUpdate(const ScriptContext& ctx) {}
    virtual void OnDestroy() {}
};
