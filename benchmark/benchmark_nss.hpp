#ifndef BENCHMARK_NSS_HPP
#define BENCHMARK_NSS_HPP

#include "../nano_signal_slot.hpp"

#include "benchmark.hpp"

class Nss : public Nano::Observer
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Nano::Signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.connect<Foo, &Foo::handler>(foo);
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject(rng);
    }
};

#endif // BENCHMARK_NSS_HPP
