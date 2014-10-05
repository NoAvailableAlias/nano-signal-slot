#ifndef BENCHMARK_JLS_HPP
#define BENCHMARK_JLS_HPP

#include "lib/jl_signal/Signal.h"

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
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject.Emit(rng);
    }
};

#endif // BENCHMARK_JLS_HPP
