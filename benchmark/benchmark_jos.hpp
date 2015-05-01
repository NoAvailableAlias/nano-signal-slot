#ifndef BENCHMARK_JOS_HPP
#define BENCHMARK_JOS_HPP

#include "lib/joanrieu/signal11/signal11.h"

#include "benchmark.hpp"

class Jos
{
    SlotScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = signal11<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        auto con = subject.connect(std::bind(&Foo::handler, &foo, std::placeholders::_1));

        // Automatically disconnect when the foo instance is destroyed
        // Benchmarks require connection management
        // Note that con must be captured by value because it is local
        foo.reg = make_slot_scope([con, &subject](void*) { subject.disconnect(con); });
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject(rng);
    }

    static void validate_assert(std::size_t);
    static double construction(std::size_t);
    static double destruction(std::size_t);
    static double connection(std::size_t);
    static double emission(std::size_t);
    static double combined(std::size_t);
};

#endif // BENCHMARK_JOS_HPP
