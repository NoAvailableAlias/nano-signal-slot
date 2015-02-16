#ifndef BENCHMARK_JLS_HPP
#define BENCHMARK_JLS_HPP

#include "lib/jeffomatic/jl_signal/src/Signal.h"

#include "benchmark.hpp"

class Jls : public jl::SignalObserver
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = jl::Signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.Connect(&foo, &Foo::handler);
    }
    template <typename Subject, typename Arg>
    static void emit_method(Subject& subject, Arg& rng)
    {
        subject.Emit(rng);
    }

    static void validate_assert(std::size_t);
    static double construction(std::size_t);
    static double destruction(std::size_t);
    static double connection(std::size_t);
    static double emission(std::size_t);
    static double combined(std::size_t);
};

#endif // BENCHMARK_JLS_HPP
