#pragma once
#include <iostream>

class Renderer; // déclaration anticipée

struct GLFWwindow;

namespace Engine {

    class WindowInstance {
    public:
        void Create(int width, int height, const char* title);
        void Update();
        bool ShouldClose() const;

        void SetRenderer(Renderer* renderer); // ← AJOUT

    private:
        GLFWwindow* m_window = nullptr;
    };

}
