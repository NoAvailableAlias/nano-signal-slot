#ifndef BENCHMARK_YAS_HPP
#define BENCHMARK_YAS_HPP

#include "lib/yassi/yassi.h"

#include "benchmark.hpp"

class Yas
{
    SlotScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Event = yassi::Signal<void(Rng&)>;
    using Signal = yassi::Emitter<Event>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.connect<Event>(foo, &Foo::handler);

        // Automatically disconnect when the foo instance is destroyed
        // Benchmarks require connection management
        foo.reg = make_slot_scope([&](void*) { subject.disconnect(foo, &Foo::handler); });
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject.emit<Event>(rng);
    }

    static void validate_assert(std::size_t);
    static double construction(std::size_t);
    static double destruction(std::size_t);
    static double connection(std::size_t);
    static double emission(std::size_t);
    static double combined(std::size_t);
};

#endif // BENCHMARK_YAS_HPP
