#include "Engine/Application.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_dx11.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <set>
#include <vector>
#include <algorithm>

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

    // Ensemble des entités sélectionnées dans l'UI
    std::set<::Entity> selectedEntities;

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

        // Overlay
        ImGui::Begin("Editor Overlay");
        ImGui::Text("Delta time: %.3f ms", dt * 1000.0f);
        ImGui::Text("FPS: %.1f", dt > 0.0f ? 1.0f / dt : 0.0f);
        if (ImGui::Button("Quit")) {
            app.setIsRunning(false);
        }
        ImGui::Checkbox("Show ImGui demo", &show_demo);
        ImGui::End();

        // Main Scene window
        ImGui::Begin("Scene");

        const auto& entities = app.GetEntities();
        ImGui::Text("Entities: %zu", entities.size());
        ImGui::Separator();

        // Deux colonnes : gauche = liste d'entités, droite = transform + actions
        ImGui::Columns(2, "scene_cols", true);

        // --- Colonne gauche : création, liste, selection multi ---
        if (ImGui::Button("Create Entity"))
        {
            ::Entity newE = app.CreateRenderableEntity();
            if (!ImGui::GetIO().KeyCtrl)
                selectedEntities.clear();
            selectedEntities.insert(newE);
        }

        ImGui::SameLine();
        if (ImGui::Button("Refresh")) { /* noop */ }

        ImGui::Separator();

        for (size_t i = 0; i < entities.size(); ++i)
        {
            ::Entity e = entities[i];
            char label[64];
            sprintf_s(label, "Entity %u", e);
            bool is_selected = selectedEntities.find(e) != selectedEntities.end();
            if (ImGui::Selectable(label, is_selected))
            {
                if (!(ImGui::GetIO().KeyCtrl))
                    selectedEntities.clear();

                if (is_selected)
                    selectedEntities.erase(e);
                else
                    selectedEntities.insert(e);
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Delete Selected"))
        {
            std::vector<::Entity> toDelete(selectedEntities.begin(), selectedEntities.end());
            for (auto ent : toDelete)
                app.DestroyEntity(ent);
            selectedEntities.clear();
        }

        ImGui::NextColumn();

        // --- Colonne droite : panneau de transform et actions individuelles ---
        if (selectedEntities.empty())
        {
            ImGui::TextWrapped("Aucune entité sélectionnée. Sélectionnez une entité à gauche pour éditer son Transform.");
        }
        else
        {
            ::Entity editEntity = *selectedEntities.begin();
            ImGui::Text("Editing: Entity %u", editEntity);

            ::Transform t = app.GetTransform(editEntity);

            float pos[3] = { t.position.x, t.position.y, t.position.z };
            float scl[3] = { t.scale.x, t.scale.y, t.scale.z };
            float rot[4] = { t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w }; // quaternion

            bool changed = false;

            // Use an ImGui table to guarantee alignment and avoid weird wrapping/spacing
            if (ImGui::BeginTable("transform_table", 2, ImGuiTableFlags_SizingStretchProp))
            {
                // Position row (uses DragFloat3 so dragging works like Unity/UE)
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Position");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-1);
                if (ImGui::DragFloat3("##position", pos, 0.01f, 0.0f, 0.0f, "%.3f"))
                    changed = true;
                ImGui::PopItemWidth();

                // Scale row
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Scale");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-1);
                if (ImGui::DragFloat3("##scale", scl, 0.01f, 0.0f, 0.0f, "%.3f"))
                    changed = true;
                ImGui::PopItemWidth();

                // Rotation row (quat) - DragFloat4 exists in ImGui
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Rotation (quat)");
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-1);
                if (ImGui::DragFloat4("##rotation", rot, 0.001f, 0.0f, 0.0f, "%.3f"))
                    changed = true;
                ImGui::PopItemWidth();

                ImGui::EndTable();
            }

            if (changed)
            {
                // Appliquer immédiatement
                t.position.x = pos[0]; t.position.y = pos[1]; t.position.z = pos[2];
                t.scale.x = scl[0]; t.scale.y = scl[1]; t.scale.z = scl[2];
                t.rotation.x = rot[0]; t.rotation.y = rot[1]; t.rotation.z = rot[2]; t.rotation.w = rot[3];
                app.SetTransform(editEntity, t);
            }

            ImGui::Separator();

            if (ImGui::Button("Delete This Entity"))
            {
                app.DestroyEntity(editEntity);
                selectedEntities.erase(editEntity);
            }

            ImGui::SameLine();
            if (ImGui::Button("Duplicate"))
            {
                ::Entity newE = app.CreateRenderableEntity();
                app.SetTransform(newE, t);
                if (!ImGui::GetIO().KeyCtrl) selectedEntities.clear();
                selectedEntities.insert(newE);
            }
        }

        ImGui::Columns(1);
        ImGui::End(); // Scene

        // Update engine
        app.Update(dt);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        app.GetRenderer().EndFrame();
    }

    // Shutdown ImGui
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    app.Shutdown();

    return 0;
}