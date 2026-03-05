#include "Engine/Application.h"

// include ImGui Obligatoire
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_dx11.h"
#include <GLFW/glfw3.h>
#include "Engine/ECS/ECS_Component.h"
#include "Engine/ECS/Systems/RenderSystem.h"

// include pour fonction nécessaire
#include <chrono>
#include <set>
#include <vector>
#include <algorithm>

// génération de fichier CPP + Headers
#include <fstream>
#include <filesystem>

// ------------------------------------------------------------
// Génération automatique des fichiers de script
// ------------------------------------------------------------
static void GenerateEntityFiles(const std::string& name)
{
    namespace fs = std::filesystem;

    fs::create_directories("Game/include");
    fs::create_directories("Game/src");

    std::string headerPath = "Game/include/" + name + ".h";
    std::string sourcePath = "Game/src/" + name + ".cpp";

    // HEADER
    std::ofstream h(headerPath);
    if (h)
    {
        h << "#pragma once\n";
        h << "#include \"Engine/ScriptAPI/IScript.h\"\n\n";
        h << "class " << name << " : public IScript\n";
        h << "{\n";
        h << "public:\n";
        h << "    void OnCreate() override;\n";
        h << "    void OnUpdate(const ScriptContext& ctx) override;\n";
        h << "    void OnDestroy() override;\n";
        h << "};\n";
    }

    // SOURCE
    std::ofstream cpp(sourcePath);
    if (cpp)
    {
        cpp << "#include \"" << name << ".h\"\n\n";

        cpp << "void " << name << "::OnCreate() {\n";
        cpp << "    // TODO\n";
        cpp << "}\n\n";

        cpp << "void " << name << "::OnUpdate(const ScriptContext& ctx) {\n";
        cpp << "    // TODO\n";
        cpp << "}\n\n";

        cpp << "void " << name << "::OnDestroy() {\n";
        cpp << "    // TODO\n";
        cpp << "}\n";
    }
}

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

    ImGui_ImplGlfw_InitForOther(window, true);
    ImGui_ImplDX11_Init(app.GetRenderer().GetDevice(), app.GetRenderer().GetContext());

    bool show_demo = false;

    std::set<::Entity> selectedEntities;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (app.getIsRunning() && !app.GetWindow().ShouldClose())
    {
        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ------------------------------------------------------------
        // Overlay
        // ------------------------------------------------------------
        ImGui::Begin("Editor Overlay");
        ImGui::Text("Delta time: %.3f ms", dt * 1000.0f);
        ImGui::Text("FPS: %.1f", dt > 0.0f ? 1.0f / dt : 0.0f);
        if (ImGui::Button("Quit")) app.setIsRunning(false);
        ImGui::Checkbox("Show ImGui demo", &show_demo);
        ImGui::End();

        // ------------------------------------------------------------
        // Scene Window
        // ------------------------------------------------------------
        ImGui::Begin("Scene");

        const auto& entities = app.GetEntities();
        ImGui::Text("Entities: %zu", entities.size());
        ImGui::Separator();

        ImGui::Columns(2, "scene_cols", true);

        // ------------------------------------------------------------
        // Colonne gauche : création + liste
        // ------------------------------------------------------------
        static char entityNameBuffer[64] = "NewEntity";
        ImGui::InputText("Entity Name", entityNameBuffer, sizeof(entityNameBuffer));

        if (ImGui::Button("Create Entity"))
        {
            std::string className = entityNameBuffer;

            // ⭐ OPTION 1: Créer l'entité (elle a déjà Name, Transform, MeshRenderer, Script)
            Entity e = app.CreateRenderableEntity();

            // Mettre à jour le Name existant au lieu de le réajouter
            Name& nameComp = app.GetComponent<Name>(e);
            nameComp.value = className;

            // Mettre à jour le Script existant au lieu de le réajouter
            Script& scriptComp = app.GetComponent<Script>(e);
            scriptComp.className = className;

            GenerateEntityFiles(className);

            // Sélection
            selectedEntities.clear();
            selectedEntities.insert(e);
        }

        ImGui::SameLine();
        if (ImGui::Button("Refresh")) {}

        ImGui::Separator();

        // Liste des entités
        for (Entity e : entities)
        {
            Name name = app.GetComponent<Name>(e);

            char label[128];
            sprintf_s(label, "%s (%u)", name.value.c_str(), e);

            bool is_selected = selectedEntities.contains(e);

            if (ImGui::Selectable(label, is_selected))
            {
                if (!ImGui::GetIO().KeyCtrl)
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
            for (Entity e : selectedEntities)
                app.DestroyEntity(e);
            selectedEntities.clear();
        }

        // ------------------------------------------------------------
        // Colonne droite : Transform + actions
        // ------------------------------------------------------------
        ImGui::NextColumn();

        if (selectedEntities.empty())
        {
            ImGui::TextWrapped("Aucune entité sélectionnée.");
        }
        else
        {
            Entity e = *selectedEntities.begin();

            Name name = app.GetComponent<Name>(e);
            ImGui::Text("Editing: %s (%u)", name.value.c_str(), e);

            Transform t = app.GetTransform(e);

            float pos[3] = { t.position.x, t.position.y, t.position.z };
            float scl[3] = { t.scale.x, t.scale.y, t.scale.z };
            float rot[4] = { t.rotation.x, t.rotation.y, t.rotation.z, t.rotation.w }; // quaternion
            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
            static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

            bool changed = false;

            if (ImGui::BeginTable("transform_table", 2, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Position");
                ImGui::TableSetColumnIndex(1);
                if (ImGui::DragFloat3("##pos", pos, 0.01f)) changed = true;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Scale");
                ImGui::TableSetColumnIndex(1);
                if (ImGui::DragFloat3("##scl", scl, 0.01f)) changed = true;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("Rotation");
                ImGui::TableSetColumnIndex(1);
                if (ImGui::DragFloat4("##rot", rot, 0.01f)) changed = true;

                // color row                              
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted("Color (RGB) [NOT IMPLEMENTED YET]");                              //not yet linked to entity color
                ImGui::TableSetColumnIndex(1);
                ImGui::PushItemWidth(-1);
                ImGui::ColorPicker3("##picker", (float*)&color, base_flags | ImGuiColorEditFlags_PickerHueWheel);
                ImGui::PopItemWidth();

                ImGui::EndTable();
            }

            if (changed)
            {
                t.position = { pos[0], pos[1], pos[2] };
                t.scale = { scl[0], scl[1], scl[2] };
                t.rotation = { rot[0], rot[1], rot[2], rot[3] };
                app.SetTransform(e, t);
            }

            ImGui::Separator();

            if (ImGui::Button("Delete This Entity"))
            {
                app.DestroyEntity(e);
                selectedEntities.clear();
            }

            ImGui::SameLine();
            if (ImGui::Button("Duplicate"))
            {
                Entity newE = app.CreateRenderableEntity();
                app.SetTransform(newE, t);

                Name& newName = app.GetComponent<Name>(newE);
                newName.value = name.value + "_Copy";

                Script& newScript = app.GetComponent<Script>(newE);
                newScript.className = newName.value;

                GenerateEntityFiles(newName.value);

                selectedEntities.clear();
                selectedEntities.insert(newE);
            }
        }

        ImGui::Columns(1);
        ImGui::End();

        // ------------------------------------------------------------
        // Update engine (inclut ScriptManager)
        // ------------------------------------------------------------
        app.Update(dt);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        app.GetRenderer().EndFrame();
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    app.Shutdown();
    return 0;
}