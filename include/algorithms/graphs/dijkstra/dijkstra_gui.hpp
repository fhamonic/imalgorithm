#ifndef IMALGORITHM_DIJKSTRA_HPP
#define IMALGORITHM_DIJKSTRA_HPP

#include <algorithm>
#include <optional>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include "algorithm_gui.hpp"

namespace ImAlgorithm {
namespace dijkstra {

class DijkstraGUI : public AlgorithmGUI {
private:
    bool play = false;
    float steps_per_s = 5;

public:
    void showControlPanel(ImVec2 pos, ImVec2 size) {
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Dijkstra Control Panel", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        // ImGui::Text("Nb Values:");
        // ImGui::SameLine();
        // ImGui::InputInt("", &length, 0);
        // ImGui::SameLine();
        // if(ImGui::Button("Generate") && length > 0) {
        //     std::random_device rnd_device;
        //     std::mt19937 mersenne_engine{rnd_device()};
        //     std::uniform_int_distribution<int> dist{min_value, max_value};
        //     values.resize(length);
        //     std::ranges::generate(values, [&dist, &mersenne_engine]() {
        //         return dist(mersenne_engine);
        //     });
        //     dijkstra_coroutine.emplace(
        //         F(values, bounds, pivot, cmp_indices, swap_indices));
        //     play = false;
        //     step_flags = NoOp;
        // }

        // ImGui::SliderFloat(
        //     "", &steps_per_s, 1.0f, 100.0f, "%.01f step/s",
        //     ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);
        // ImGui::SameLine();
        // if(ImGui::Button("Play")) {
        //     play = true;
        // }
        // ImGui::SameLine();
        // if(ImGui::Button("Pause")) {
        //     play = false;
        // }
        // ImGui::SameLine();
        // if(ImGui::Button("Step forward")) {
        //     play = false;
        //     if(dijkstra_coroutine.has_value()) {
        //         if(!dijkstra_coroutine->finished()) {
        //             dijkstra_coroutine->advance_to_next_step();
        //             step_flags = dijkstra_coroutine->current_step();
        //         }
        //     }
        // }

        // ImGui::Checkbox("Highlight pivot", &b_highlight_pivot);
        // ImGui::SameLine();
        // ImGui::Checkbox("Highlight comparison", &b_highlight_cmp);
        // ImGui::SameLine();
        // ImGui::Checkbox("Highlight swap", &b_highlight_swap);

        ImGui::End();
    }
    void showGraph(ImVec2 pos, ImVec2 size) const {
        ImGuiIO io = ImGui::GetIO();
        static bool dragging = false;

        static ImVec2 offset = ImVec2(0, 0);
        ImVec2 display_offset = offset;
        static float zoom = 1.0f;

        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::Begin("Test", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImVec2 content_min_p =
            ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
        ImVec2 content_max_p =
            ImGui::GetWindowContentRegionMax() + ImGui::GetWindowPos();
        ImVec2 content_size = content_max_p - content_min_p;

        ImGui::InvisibleButton("canvas", content_size);

        if(ImGui::IsItemHovered() && io.MouseWheel != 0.0f) {
            const float scale = 1 + io.MouseWheel / 20;
            zoom *= scale;
            ImVec2 mouse_pos =
                (io.MousePos - content_min_p - ImGui::GetMouseDragDelta()) -
                offset;
            offset += mouse_pos - scale * mouse_pos;
            display_offset = offset;
        }
        if(ImGui::IsItemHovered() && ImGui::IsMouseDown(0)) {
            dragging = true;
        }
        if(dragging) display_offset += ImGui::GetMouseDragDelta();
        if(ImGui::IsMouseReleased(0)) {
            dragging = false;
            offset = display_offset;
        }

        ImDrawList * draw_list = ImGui::GetWindowDrawList();

        const ImVec2 center = ImVec2(100, 100);
        const float radius = 50.0f;

        draw_list->AddCircle(content_min_p + display_offset + zoom * center,
                             zoom * radius, IM_COL32(255, 0, 0, 255),
                             std::clamp(int(1.5 * zoom * radius), 6, 64), 2.0f);

        draw_list->AddCircle(content_min_p + display_offset, zoom * 5,
                             IM_COL32(0, 255, 0, 255), 12, 2.0f);

        ImGui::End();
    }
    void show(ImVec2 pos, ImVec2 size) {
        showControlPanel(ImVec2(pos.x, pos.y + size.y - 100),
                         ImVec2(size.x, 100));
        // if(dijkstra_coroutine.has_value()) {
        //     if(play) {
        // static float spare_time = 0.0f;
        // ImGuiIO & io = ImGui::GetIO();
        // int nb_steps = (spare_time + io.DeltaTime) * steps_per_s;
        // spare_time =
        //     (spare_time + io.DeltaTime) * steps_per_s - nb_steps;

        // for(int i = 0; i < nb_steps; ++i) {
        //     if(dijkstra_coroutine->finished()) break;
        //     dijkstra_coroutine->advance_to_next_step();
        // }
        // play = !dijkstra_coroutine->finished();
        // }
        showGraph(pos, ImVec2(size.x, size.y - 100));
        // }
    };
    const char * name() const { return "Dijkstra"; }
};

}  // namespace dijkstra
}  // namespace ImAlgorithm

#endif  // IMALGORITHM_DIJKSTRA_HPP