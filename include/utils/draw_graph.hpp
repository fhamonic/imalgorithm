
ImGui::Begin("Quicksort Control Panel");
        ImDrawList * draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(ImVec2(), ImGui::GetIO().DisplaySize,
                                 IM_COL32(255, 255, 255, 255));
        ImVec2 p = ImGui::GetCursorScreenPos();

        draw_list->Flags |= ImDrawListFlags_AntiAliasedLinesUseTex;

        ImVec2 center(200.0f, 200.0f);
        static constexpr float r = 50.0f;
        static constexpr int nb_segments =
            static_cast<int>(r * r / (20.0f * 3.1415f));

        draw_list->AddCircle(ImVec2(p.x + center.x, p.y + center.y), r,
                             IM_COL32(128, 128, 128, 64), 0, 3.0f);
        draw_list->AddCircle(ImVec2(p.x + center.x, p.y + center.y), r,
                             IM_COL32(0, 0, 0, 255), 0, 2.5f);
        draw_list->AddText(
            NULL, 50.0f,
            ImVec2(p.x + center.x - 75 / 2, p.y + center.y - 75 / 2),
            IM_COL32(0, 0, 0, 255), "123456789");
        // Advance the ImGui cursor to claim space in the window (otherwise the
        // window will appear small and needs to be resized)
        ImGui::Dummy(ImVec2(200, 200));
        ImGui::End();