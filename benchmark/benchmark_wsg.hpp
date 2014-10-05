#ifndef BENCHMARK_WSG_HPP
#define BENCHMARK_WSG_HPP

#include "lib\winglot\Signals\W_Signal.h"

#include "benchmark.hpp"

class Wsg : public W::Slot
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = W::Signal<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.connect(&foo, &Foo::handler);
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject.emit(rng);
    }
};

#endif // BENCHMARK_WSG_HPP
