#include <memory>
#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

// #include "implot.h"

#include "algorithms/algorithm.hpp"

#include "algorithms/quicksort.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    ImGuiIO & io = ImGui::GetIO();
    ImFont * pFont = io.Fonts->AddFontFromFileTTF(
        "build/_deps/imgui-src/misc/fonts/DroidSans.ttf", 12.0f);
    ImFont * pZoomedFont = io.Fonts->AddFontFromFileTTF(
        "build/_deps/imgui-src/misc/fonts/DroidSans.ttf", 96.0f);
    ImGui::SFML::UpdateFontTexture();

    std::optional<std::unique_ptr<Algorithm>> algorithm;

    window.setTitle("ImAlgorithm");
    window.resetGLStates();  // call it if you only draw ImGui. Otherwise not
                             // needed.
    sf::Clock deltaClock;
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        float menu_bar_height;
        if(ImGui::BeginMainMenuBar()) {
            if(ImGui::BeginMenu("Algorithms")) {
                if(ImGui::MenuItem("-- None --")) algorithm.reset();
                if(ImGui::MenuItem("Quicksort"))
                    algorithm.emplace(std::make_unique<QuickSort>());
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            menu_bar_height = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }

        if(algorithm.has_value()) {
            ImGuiIO & io = ImGui::GetIO();
            algorithm->get()->show(
                ImVec2(0, menu_bar_height),
                ImVec2(io.DisplaySize.x, io.DisplaySize.y - menu_bar_height));
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}