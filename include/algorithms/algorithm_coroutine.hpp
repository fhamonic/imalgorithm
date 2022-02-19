#ifndef IMALGORITHM_ALGORITHM_COROUTINE_HPP
#define IMALGORITHM_ALGORITHM_COROUTINE_HPP

#include <coroutine>

namespace ImAlgorithm {

template <typename T>
struct AlgorithmCoroutine {
    using value_type = T;
    struct promise_type {
        using value_type = T;
        value_type current_value;
        std::suspend_always yield_value(value_type value) noexcept {
            this->current_value = value;
            return {};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        AlgorithmCoroutine get_return_object() noexcept {
            return AlgorithmCoroutine{this};
        };
        void unhandled_exception() noexcept { std::terminate(); }
        void return_void() noexcept {}
    };

    AlgorithmCoroutine & advance_to_next_step() noexcept {
        _handle.resume();
        return *this;
    }
    bool finished() const noexcept { return _handle.done(); }
    value_type current_step() const noexcept {
        return _handle.promise().current_value;
    }

    AlgorithmCoroutine(AlgorithmCoroutine const &) = delete;
    AlgorithmCoroutine(AlgorithmCoroutine && rhs)
        : _handle(std::exchange(rhs._handle, nullptr)) {}

    ~AlgorithmCoroutine() {
        if(_handle) _handle.destroy();
    }

private:
    explicit AlgorithmCoroutine(promise_type * p)
        : _handle(std::coroutine_handle<promise_type>::from_promise(*p)) {}

    std::coroutine_handle<promise_type> _handle;
};

}  // namespace ImAlgorithm

#endif  // IMALGORITHM_ALGORITHM_COROUTINE_HPP