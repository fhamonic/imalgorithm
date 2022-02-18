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
    bool started;
    int length = 100;

    std::vector<int> values;
    int min_value;
    int max_value;

    float getRatio(int value) {
        return static_cast<float>(value - min_value) / (max_value - min_value);
    }

public:
    void showControlPanel() {
        ImGuiIO & io = ImGui::GetIO();
        ImVec2 main_window_size = io.DisplaySize;
        ImGui::SetNextWindowSize(ImVec2(main_window_size.x, 100));
        ImGui::SetNextWindowPos(ImVec2(0, main_window_size.y - 100));

        ImGui::Begin("Quicksort Control Panel");

        ImGui::Text("Nb Values:");
        ImGui::SameLine();
        ImGui::InputInt("", &length, 0);
        ImGui::SameLine();
        if(ImGui::Button("Generate")) {
            std::random_device rnd_device;
            std::mt19937 mersenne_engine{rnd_device()};
            std::uniform_int_distribution<int> dist{1, 500};
            values.resize(length);
            std::ranges::generate(values, [&dist, &mersenne_engine]() {
                return dist(mersenne_engine);
            });
            auto bounds = std::ranges::minmax(values);
            min_value = bounds.min;
            max_value = bounds.max;
            started = true;
        }

        if(ImGui::Button("Reset")) {
            started = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Step forward")) {
        }

        ImGui::End();
    }
    void showValues() {
        ImGuiIO & io = ImGui::GetIO();
        ImVec2 main_window_size = io.DisplaySize;
        float window_width = io.DisplaySize.x;
        float window_height = io.DisplaySize.y;

        ImGui::SetNextWindowSize(ImVec2(window_width, window_height - 100));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Values");
        ImDrawList * draw_list = ImGui::GetWindowDrawList();
        const ImVec2 p = ImGui::GetCursorScreenPos();

        float width = window_width / values.size();
        for(int i = 0; i < values.size(); ++i) {
            float ratio = getRatio(values[i]);
            float height = ratio * window_height;
            draw_list->AddRectFilled(
                ImVec2(p.x + (i + 1) * width, p.y + window_height - height),
                ImVec2(p.x + i * width, p.y + window_height),
                IM_COL32(255 - ratio * 255, 0, ratio * 255, 255));
        }

        ImGui::End();
    }
    void show() {
        showControlPanel();
        if(started) showValues();
    };
};

#endif  // IMALGORITHM_QUICKSORT_HPP