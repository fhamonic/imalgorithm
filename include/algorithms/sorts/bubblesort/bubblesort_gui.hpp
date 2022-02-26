#ifndef IMALGORITHM_BUBBLESORT_GUI_HPP
#define IMALGORITHM_BUBBLESORT_GUI_HPP

#include <algorithm>
#include <optional>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include "algorithm_gui.hpp"
#include "algorithms/sorts/bubblesort/bubblesort_coroutine.hpp"

namespace ImAlgorithm {
namespace bubblesort {

class BubbleSortGUI : public AlgorithmGUI {
private:
    bool play = false;
    bool b_highlight_cmp = true;
    bool b_highlight_swap = true;
    float steps_per_s = 5;

    int length = 100;
    int min_value = 0;
    int max_value = 500;

    std::vector<int> values;
    std::pair<std::size_t, std::size_t> cmp_indices;
    std::pair<std::size_t, std::size_t> swap_indices;

    std::optional<BubblesortCoroutine> bubblesort_coroutine;

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
            bubblesort_coroutine.emplace(
                bubblesort(values, cmp_indices, swap_indices));
            play = false;
        }

        ImGui::SliderFloat(
            "", &steps_per_s, 1.0f, 1000.0f, "%.01f step/s",
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
            if(bubblesort_coroutine.has_value()) {
                if(!bubblesort_coroutine->finished()) {
                    bubblesort_coroutine->advance_to_next_step();
                }
            }
        }

        ImGui::Checkbox("Highlight comparison", &b_highlight_cmp);
        ImGui::SameLine();
        ImGui::Checkbox("Highlight swap", &b_highlight_swap);

        ImGui::End();
    }
    void showValues(ImVec2 pos, ImVec2 size) const {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Values", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImDrawList * draw_list = ImGui::GetWindowDrawList();

        ImVec2 content_min_p =
            ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
        ImVec2 content_max_p =
            ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos();
        ImVec2 content_size = content_max_p - content_min_p;

        const float width = content_size.x / values.size();

        auto draw_value_rect = [&](std::size_t i, ImU32 col) {
            const float height = content_size.y *
                                 static_cast<float>(values[i] - min_value) /
                                 (max_value - min_value);
            draw_list->AddRectFilled(
                ImVec2(content_min_p.x + (i + 1) * width,
                       content_max_p.y - height),
                ImVec2(content_min_p.x + i * width, content_max_p.y), col);
        };

        for(std::size_t i = 0; i < values.size(); ++i) {
            const float c = 200 * static_cast<float>(values[i] - min_value) /
                            (max_value - min_value);
            draw_value_rect(i, IM_COL32(255 - c, 0, c, 255));
        }

        BubblesortStepFlags step_flags = bubblesort_coroutine->current_step();

        if(b_highlight_cmp && step_flags & HighlightCmp) {
            draw_value_rect(cmp_indices.first, IM_COL32(255, 255, 255, 255));
            draw_value_rect(cmp_indices.second, IM_COL32(255, 255, 255, 255));
        }
        if(b_highlight_swap && step_flags & HighlightSwap) {
            draw_value_rect(swap_indices.first, IM_COL32(255, 255, 255, 255));
            draw_value_rect(swap_indices.second, IM_COL32(255, 255, 255, 255));
        }

        ImGui::End();
    }
    void show(ImVec2 pos, ImVec2 size) {
        showControlPanel(ImVec2(pos.x, pos.y + size.y - 100),
                         ImVec2(size.x, 100));
        if(bubblesort_coroutine.has_value()) {
            if(play) {
                static float spare_time = 0.0f;
                ImGuiIO & io = ImGui::GetIO();
                int nb_steps = (spare_time + io.DeltaTime) * steps_per_s;
                spare_time =
                    (spare_time + io.DeltaTime) * steps_per_s - nb_steps;

                for(int i = 0; i < nb_steps; ++i) {
                    if(bubblesort_coroutine->finished()) break;
                    bubblesort_coroutine->advance_to_next_step();
                }
                play = !bubblesort_coroutine->finished();
            }
            showValues(pos, ImVec2(size.x, size.y - 100));
        }
    };
    const char * name() const { return "BubbleSort"; }
};

}  // namespace bubblesort
}  // namespace ImAlgorithm

#endif  // IMALGORITHM_BUBBLESORT_GUI_HPP