#ifndef IMALGORITHM_QUICKSORT_COROUTINE_HPP
#define IMALGORITHM_QUICKSORT_COROUTINE_HPP

#include <algorithm>
#include <random>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

#include "algorithms/algorithm_coroutine.hpp"

namespace ImAlgorithm {
namespace quicksort {

using QuicksortStepFlags = int;
enum QuicksortStepFlags_ : int {
    QuicksortStepFlags_Nop = 0,
    QuicksortStepFlags_HighlightCmp = 1,
    QuicksortStepFlags_HighlightPivot = 2,
    QuicksortStepFlags_DisplayBounds = 4
};

using QuicksortCoroutine = AlgorithmCoroutine<QuicksortStepFlags>;

QuicksortCoroutine quicksort(
    std::vector<int> & values,
    std::stack<std::pair<std::size_t, std::size_t>> & bounds,
    std::size_t & pivot_index,
    std::pair<std::size_t, std::size_t> & cmp_indices) noexcept {
    // std::random_device rnd_device;
    // std::mt19937 mersenne_engine{rnd_device()};

    bounds.emplace(0, values.size());

    while(!bounds.empty()) {
        const auto [begin, end] = bounds.top();
        co_yield QuicksortStepFlags_Nop;
        if(end - begin <= 1) {
            bounds.pop();
            continue;
        }
        // std::uniform_int_distribution<std::size_t> dist{begin, end};
        // pivot_index = dist(mersenne_engine);
        // swap_indices = std::make_pair(pivot_index, end - 1);
        // co_yield QuicksortStepFlags_Nop;
        // std::swap(values[pivot_index], values[end - 1]);
        pivot_index = end - 1;
        co_yield QuicksortStepFlags_HighlightPivot;

        std::size_t cursor = begin;
        for(std::size_t i = begin; i < pivot_index; ++i) {
            if(values[i] < values[pivot_index]) {
                cmp_indices = std::make_pair(i, cursor);
                std::swap(values[i], values[cursor]);
                co_yield (QuicksortStepFlags_HighlightPivot | QuicksortStepFlags_HighlightCmp);
                ++cursor;
            }
        }
        std::swap(values[cursor], values[pivot_index]);
        pivot_index = cursor;
        co_yield QuicksortStepFlags_HighlightPivot;

        bounds.pop();
        bounds.emplace(pivot_index + 1, end);
        bounds.emplace(begin, pivot_index);
    }
    co_yield QuicksortStepFlags_Nop;
}

}  // namespace Quicksort
}  // namespace ImAlgorithm

#endif  // IMALGORITHM_QUICKSORT_COROUTINE_HPP