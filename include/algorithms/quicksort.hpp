#ifndef IMALGORITHM_QUICKSORT_HPP
#define IMALGORITHM_QUICKSORT_HPP

#include <algorithm>
// #include <iostream>
#include <random>
#include <ranges>
#include <vector>

#include "algorithms/algorithm.hpp"

class QuickSort : public Algorithm {
private:
    bool show_values;

    int length = 100;
    float speed = 10;

    std::vector<int> values;
    int min_value = 0;
    int max_value = 500;

    float getRatio(int value) {
        return static_cast<float>(value - min_value) / (max_value - min_value);
    }

public:
    void showControlPanel(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Quicksort Control Panel");

        ImGui::Text("Nb Values:");
        ImGui::SameLine();
        ImGui::InputInt("", &length, 0);
        ImGui::SameLine();
        if(ImGui::Button("Generate") && length > 0) {
            std::random_device rnd_device;
            std::mt19937 mersenne_engine{rnd_device()};
            std::uniform_int_distribution<int> dist{min_value, max_value};
            values.resize(length);
            std::ranges::generate(values, [&dist, &mersenne_engine]() {
                return dist(mersenne_engine);
            });
            show_values = true;
        }

        ImGui::SliderFloat(
            "", &speed, 0.1f, 1000.0f, "%.01f step/s",
            ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
        ImGui::SameLine();
        if(ImGui::Button("Play")) {
        }
        ImGui::SameLine();
        if(ImGui::Button("Pause")) {
        }
        ImGui::SameLine();
        if(ImGui::Button("Step forward")) {
        }

        ImGui::End();
    }
    void showValues(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Values");
        ImDrawList * draw_list = ImGui::GetWindowDrawList();

        ImVec2 content_min_p = ImGui::GetWindowContentRegionMin();
        ImVec2 content_max_p = ImGui::GetWindowContentRegionMax();
        content_min_p.x += ImGui::GetWindowPos().x;
        content_min_p.y += ImGui::GetWindowPos().y;
        content_max_p.x += ImGui::GetWindowPos().x;
        content_max_p.y += ImGui::GetWindowPos().y;

        const float content_width = (content_max_p.x - content_min_p.x);
        const float content_height = (content_max_p.y - content_min_p.y);

        const float width = content_width / values.size();
        for(int i = 0; i < values.size(); ++i) {
            float ratio = getRatio(values[i]);
            float height = ratio * content_height;
            draw_list->AddRectFilled(
                ImVec2(content_min_p.x + (i + 1) * width,
                       content_max_p.y - height),
                ImVec2(content_min_p.x + i * width,
                       content_min_p.y + content_height),
                IM_COL32(255 - ratio * 255, 0, ratio * 255, 255));
        }

        ImGui::End();
    }
    void show(ImVec2 pos, ImVec2 size) {
        showControlPanel(ImVec2(pos.x, pos.y + size.y - 100),
                         ImVec2(size.x, 100));
        if(show_values) {
            showValues(pos, ImVec2(size.x, size.y - 100));
        }
    };
};

#endif  // IMALGORITHM_QUICKSORT_HPP