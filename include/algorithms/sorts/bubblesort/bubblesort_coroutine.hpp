#ifndef IMALGORITHM_BUBBLESORT_COROUTINE_HPP
#define IMALGORITHM_BUBBLESORT_COROUTINE_HPP

#include <algorithm>
#include <random>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

#include "algorithm_coroutine.hpp"

namespace ImAlgorithm {
namespace bubblesort {

using BubblesortStepFlags = int;
enum BubblesortStepFlag : int { NoOp = 0, HighlightCmp = 1, HighlightSwap = 2 };

using BubblesortCoroutine = AlgorithmCoroutine<BubblesortStepFlags>;

BubblesortCoroutine bubblesort(
    std::vector<int> & values,
    std::pair<std::size_t, std::size_t> & cmp_indices,
    std::pair<std::size_t, std::size_t> & swap_indices) noexcept {
    bool b_continue = true;

    while(b_continue) {
        b_continue = false;
        for(std::size_t i = 1; i < values.size(); ++i) {
            cmp_indices = std::make_pair(i - 1, i);
            co_yield(HighlightCmp);
            if(values[i - 1] > values[i]) {
                b_continue = true;
                swap_indices = std::make_pair(i - 1, i);
                co_yield(HighlightSwap);
                std::swap(values[i - 1], values[i]);
                co_yield(HighlightSwap);
            }
        }
    }

    co_yield NoOp;
}

}  // namespace bubblesort
}  // namespace ImAlgorithm

#endif  // IMALGORITHM_BUBBLESORT_COROUTINE_HPP