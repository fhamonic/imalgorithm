#ifndef IMALGORITHM_QUICKSORT_HPP
#define IMALGORITHM_QUICKSORT_HPP

#include <algorithm>
#include <optional>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include "algorithms/algorithm_gui.hpp"
#include "algorithms/quicksort/quicksort_coroutine.hpp"

namespace ImAlgorithm {

class QuickSortGUI : public AlgorithmGUI {
private:
    bool play = false;
    float steps_per_s = 10;

    int length = 100;
    int min_value = 0;
    int max_value = 500;

    std::vector<int> values;
    std::stack<std::pair<std::size_t, std::size_t>> bounds;
    std::size_t pivot;
    std::pair<std::size_t, std::size_t> swap_indices;

    std::optional<QuicksortCoroutine> quicksort_coroutine;

public:
    void showControlPanel(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Quicksort Control Panel", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

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
            quicksort_coroutine.emplace(
                quicksort(values, bounds, pivot, swap_indices));
            play = false;
        }

        ImGui::SliderFloat(
            "", &steps_per_s, 1.0f, 100.0f, "%.01f step/s",
            ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
        ImGui::SameLine();
        if(ImGui::Button("Play")) {
            play = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Pause")) {
            play = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Step forward")) {
            play = false;
            if(quicksort_coroutine.has_value()) {
                if(!quicksort_coroutine->finished())
                    quicksort_coroutine->advance_to_next_step();
            }
        }

        ImGui::End();
    }
    void showValues(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Values", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
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
        for(std::size_t i = 0; i < values.size(); ++i) {
            const float ratio = static_cast<float>(values[i] - min_value) /
                                (max_value - min_value);
            const float height = ratio * content_height;
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
        if(quicksort_coroutine.has_value()) {
            if(play) {
                static float spare_time = 0.0f;
                ImGuiIO & io = ImGui::GetIO();
                int nb_steps = (spare_time + io.DeltaTime) * steps_per_s;
                spare_time = (spare_time + io.DeltaTime) * steps_per_s - nb_steps;
                int i = 0;
                while(!quicksort_coroutine->finished() && i < nb_steps) {
                    quicksort_coroutine->advance_to_next_step();
                    ++i;
                }
                play = !quicksort_coroutine->finished();
            }
            showValues(pos, ImVec2(size.x, size.y - 100));
        }
    };
};

}  // namespace ImAlgorithm

#endif  // IMALGORITHM_QUICKSORT_HPP