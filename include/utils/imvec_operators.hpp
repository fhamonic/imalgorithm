#ifndef IMALGORITHM_IMVEC_OPERATORS_HPP
#define IMALGORITHM_IMVEC_OPERATORS_HPP

#include <imgui.h>

namespace ImAlgorithm {

inline ImVec2 operator+(const ImVec2 & v1, const ImVec2 & v2) {
    return ImVec2(v1.x + v2.x, v1.y + v2.y);
}
inline ImVec2 & operator+=(ImVec2 & v1, const ImVec2 & v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}
inline ImVec2 operator-(const ImVec2 & v1, const ImVec2 & v2) {
    return ImVec2(v1.x - v2.x, v1.y - v2.y);
}
inline ImVec2 & operator-=(ImVec2 & v1, const ImVec2 & v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

}  // namespace ImAlgorithm

#endif  // IMALGORITHM_IMVEC_OPERATORS_HPP