#ifndef BENCHMARK_PSG_HPP
#define BENCHMARK_PSG_HPP

#include "lib\pbhogan\Signals\Signal.h"

#include "benchmark.hpp"

class Psg
{
    SlotScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Gallant::Signal1<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.Connect(&foo, &Foo::handler);

        // Automatically disconnect when the foo instance is destroyed
        // Benchmarks require connection management
        foo.reg = make_slot_scope([&](void*) { subject.Disconnect(&foo, &Foo::handler); });
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
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

#endif // BENCHMARK_PSG_HPP
