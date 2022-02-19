#ifndef IMALGORITHM_ALGORITHM_HPP
#define IMALGORITHM_ALGORITHM_HPP

#include <imgui.h>

class Algorithm {
public:
    virtual void show(ImVec2 pos, ImVec2 size) = 0;
};

#endif  // IMALGORITHM_ALGORITHM_HPP