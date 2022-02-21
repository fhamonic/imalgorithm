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
enum QuicksortStepFlag : int {
    NoOp = 0,
    HighlightCmp = 1,
    HighlightSwap = 2,
    HighlightPivot = 4,
    DisplayBounds = 8
};

using QuicksortCoroutine = AlgorithmCoroutine<QuicksortStepFlags>;

QuicksortCoroutine quicksort_lomuto(
    std::vector<int> & values,
    std::stack<std::pair<std::size_t, std::size_t>> & bounds,
    std::size_t & pivot_index,
    std::pair<std::size_t, std::size_t> & cmp_indices,
    std::pair<std::size_t, std::size_t> & swap_indices) noexcept {
    bounds.emplace(0, values.size());

    while(!bounds.empty()) {
        const auto [begin, end] = bounds.top();
        if(end - begin <= 1) {
            bounds.pop();
            continue;
        }
        pivot_index = end - 1;
        co_yield HighlightPivot;

        std::size_t cursor = begin;
        for(std::size_t i = begin; i < pivot_index; ++i) {
            cmp_indices = std::make_pair(i, pivot_index);
            co_yield(HighlightPivot | HighlightCmp);
            if(values[i] < values[pivot_index]) {
                swap_indices = std::make_pair(i, cursor);
                co_yield(HighlightPivot | HighlightSwap);
                std::swap(values[i], values[cursor]);
                co_yield(HighlightPivot | HighlightSwap);
                ++cursor;
            }
        }
        std::swap(values[cursor], values[pivot_index]);
        pivot_index = cursor;
        co_yield HighlightPivot;

        bounds.pop();
        bounds.emplace(pivot_index + 1, end);
        bounds.emplace(begin, pivot_index);
    }
    co_yield NoOp;
}

// TODO : debug a degenrative case
QuicksortCoroutine quicksort_hoare(
    std::vector<int> & values,
    std::stack<std::pair<std::size_t, std::size_t>> & bounds,
    std::size_t & pivot_index,
    std::pair<std::size_t, std::size_t> & cmp_indices,
    std::pair<std::size_t, std::size_t> & swap_indices) noexcept {
    bounds.emplace(0, values.size());

    while(!bounds.empty()) {
        const auto [begin, end] = bounds.top();
        if(end - begin <= 1) {
            bounds.pop();
            continue;
        }
        pivot_index = end - 1;
        co_yield HighlightPivot;

        std::size_t left_cursor = begin;
        std::size_t right_cursor = end - 2;
        for(;;) {
            cmp_indices = std::make_pair(left_cursor, pivot_index);
            co_yield(HighlightPivot | HighlightCmp);
            while(left_cursor < end &&
                  values[left_cursor] < values[pivot_index]) {
                ++left_cursor;
                cmp_indices = std::make_pair(left_cursor, pivot_index);
                co_yield(HighlightPivot | HighlightCmp);
            }

            cmp_indices = std::make_pair(right_cursor, pivot_index);
            co_yield(HighlightPivot | HighlightCmp);
            while(right_cursor > begin &&
                  values[right_cursor] >= values[pivot_index]) {
                --right_cursor;
                cmp_indices = std::make_pair(right_cursor, pivot_index);
                co_yield(HighlightPivot | HighlightCmp);
            }
            if(right_cursor <= left_cursor) {
                break;
            }

            swap_indices = std::make_pair(left_cursor, right_cursor);
            co_yield(HighlightPivot | HighlightSwap);
            std::swap(values[left_cursor], values[right_cursor]);
            co_yield(HighlightPivot | HighlightSwap);
        }

        std::swap(values[left_cursor], values[pivot_index]);
        pivot_index = left_cursor;
        co_yield HighlightPivot;

        bounds.pop();
        bounds.emplace(pivot_index + 1, end);
        bounds.emplace(begin, pivot_index);
    }
    co_yield NoOp;
}

}  // namespace quicksort
}  // namespace ImAlgorithm

#endif  // IMALGORITHM_QUICKSORT_COROUTINE_HPP