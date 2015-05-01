#ifndef BENCHMARK_ASG_HPP
#define BENCHMARK_ASG_HPP

#include "lib/amc522/signal11/Signal.h"

#include "benchmark.hpp"

class Asg
{
    Signal11::ConnectionScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Signal11::Signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        foo.reg += subject.connect(&foo, &Foo::handler);
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject.emit(rng);
    }

    static void validate_assert(std::size_t);
    static double construction(std::size_t);
    static double destruction(std::size_t);
    static double connection(std::size_t);
    static double emission(std::size_t);
    static double combined(std::size_t);
};

#endif // BENCHMARK_ASG_HPP
