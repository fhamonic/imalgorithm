#ifndef IMALGORITHM_QUICKSORT_COROUTINE_HPP
#define IMALGORITHM_QUICKSORT_COROUTINE_HPP

#include <algorithm>
#include <random>
#include <ranges>
#include <vector>

#include "algorithms/algorithm_coroutine.hpp"

namespace ImAlgorithm {

using QuicksortCoroutine = AlgorithmCoroutine<int>;

QuicksortCoroutine quicksort(
    std::vector<int> & values, std::size_t & pivot,
    std::pair<std::size_t, std::size_t> & swap_indices) noexcept {
        std::ranges::sort(values);
        co_yield 1;
    }

}  // namespace ImAlgorithm

#endif  // IMALGORITHM_QUICKSORT_COROUTINE_HPP