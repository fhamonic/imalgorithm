#ifndef IMALGORITHM_QUICKSORT_COROUTINE_HPP
#define IMALGORITHM_QUICKSORT_COROUTINE_HPP

#include <algorithm>
#include <random>
#include <ranges>
#include <stack>
#include <vector>

#include "algorithms/algorithm_coroutine.hpp"

namespace ImAlgorithm {

using QuicksortCoroutine = AlgorithmCoroutine<int>;

QuicksortCoroutine quicksort(
    std::vector<int> & values,
    std::stack<std::pair<std::size_t, std::size_t>> & bounds,
    std::size_t & pivot_index,
    std::pair<std::size_t, std::size_t> & swap_indices) noexcept {
    // std::random_device rnd_device;
    // std::mt19937 mersenne_engine{rnd_device()};

    bounds.emplace(0, values.size());

    while(!bounds.empty()) {
        const auto [begin, end] = bounds.top();
        co_yield 0;
        if(end - begin <= 1) {
            bounds.pop();
            continue;
        }
        // std::uniform_int_distribution<std::size_t> dist{begin, end};
        // pivot_index = dist(mersenne_engine);
        // swap_indices = std::make_pair(pivot_index, end - 1);
        // co_yield 0;
        // std::swap(values[pivot_index], values[end - 1]);
        pivot_index = end - 1;
        co_yield 0;

        std::size_t cursor = begin;
        for(std::size_t i = begin; i < pivot_index; ++i) {
            if(values[i] < values[pivot_index]) {
                std::swap(values[i], values[cursor]);
                co_yield 0;
                ++cursor;
            }
        }
        std::swap(values[cursor], values[pivot_index]);
        pivot_index = cursor;
        co_yield 0;

        bounds.pop();
        bounds.emplace(pivot_index + 1, end);
        bounds.emplace(begin, pivot_index);
    }
    co_yield 0;
}

}  // namespace ImAlgorithm

#endif  // IMALGORITHM_QUICKSORT_COROUTINE_HPP