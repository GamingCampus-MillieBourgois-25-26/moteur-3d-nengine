#include "Engine/Application.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_dx11.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdio> // for std::sprintf

int main()
{
    Engine::Application app;
    app.Init();

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    GLFWwindow* window = app.GetWindow().GetGLFWwindow();

    // Initialise backend GLFW (generic) and DX11
    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplDX11_Init(app.GetRenderer().GetDevice(), app.GetRenderer().GetContext());

    bool show_demo = false;

    // selected entity sentinel (types are in global namespace)
    ::Entity selectedEntity = static_cast<::Entity>(-1);

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (app.getIsRunning() && !app.GetWindow().ShouldClose())
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - lastTime;
        float dt = elapsed.count();
        lastTime = now;

        // Start ImGui frame (we build UI before rendering the scene)
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Scene window: create entity + list
        ImGui::Begin("Scene");
        if (ImGui::Button("Create Renderable Entity")) {
                ::Entity e = app.CreateRenderableEntity();
            selectedEntity = e; // auto-select newly created
        }
        ImGui::Separator();

        const auto& entities = app.GetEntities();
        for (size_t i = 0; i < entities.size(); ++i)
        {
            ::Entity e = entities[i];
            char label[64];
            std::sprintf(label, "Entity %u##%u", static_cast<unsigned>(e), static_cast<unsigned>(e));
            bool isSelected = (selectedEntity == e);
            if (ImGui::Selectable(label, isSelected)) {
                selectedEntity = e;
            }
        }

        ImGui::End();

        // Inspector window: edit Transform of selected entity
        ImGui::Begin("Inspector");
        if (selectedEntity == static_cast<::Entity>(-1)) {
            ImGui::Text("No entity selected");
        }
        else
        {
            // Fetch a copy of the transform (types in global namespace)
            ::Transform tr = app.GetTransform(selectedEntity);

            float pos[3] = { tr.position.x, tr.position.y, tr.position.z };
            if (ImGui::DragFloat3("Position", pos, 0.1f)) {
                tr.position.x = pos[0];
                tr.position.y = pos[1];
                tr.position.z = pos[2];
                app.SetTransform(selectedEntity, tr);
            }

            float scale[3] = { tr.scale.x, tr.scale.y, tr.scale.z };
            if (ImGui::DragFloat3("Scale", scale, 0.01f, 0.001f, 100.0f)) {
                tr.scale.x = scale[0];
                tr.scale.y = scale[1];
                tr.scale.z = scale[2];
                app.SetTransform(selectedEntity, tr);
            }

            float quat[4] = { tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w };
            if (ImGui::InputFloat4("Rotation (quat)", quat)) {
                tr.rotation.x = quat[0];
                tr.rotation.y = quat[1];
                tr.rotation.z = quat[2];
                tr.rotation.w = quat[3];
                // If your quaternion class requires Normalize after manual change:
                tr.rotation.Normalize();
                app.SetTransform(selectedEntity, tr);
            }

            ImGui::Separator();
            if (ImGui::Button("Remove Entity (not implemented)")) {
                // Optionnel : implémenter DestroyEntity() si souhaité
            }
        }
        ImGui::End();

        // Example UI (stats / quit)
        ImGui::Begin("Editor Overlay");
        ImGui::Text("Delta time: %.3f ms", dt * 1000.0f);
        ImGui::Text("FPS: %.1f", dt > 0.0f ? 1.0f / dt : 0.0f);
        if (ImGui::Button("Quit")) {
            app.setIsRunning(false);
        }
        ImGui::Checkbox("Show ImGui demo", &show_demo);
        ImGui::End();

        if (show_demo) {
            // show_demo requires imgui_demo.cpp in the build. If you didn't add it, comment out this line.
            // ImGui::ShowDemoWindow(&show_demo);
        }

        // Update engine (scene draw happens here; it calls BeginFrame + draw, but NOT EndFrame)
        app.Update(dt);

        // Render ImGui on top of the scene
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present (EndFrame) after ImGui has been drawn
        app.GetRenderer().EndFrame();
    }

    // Shutdown ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    app.Shutdown();

    return 0;
}