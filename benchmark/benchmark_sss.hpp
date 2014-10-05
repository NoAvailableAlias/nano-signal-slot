#ifndef BENCHMARK_SSS_HPP
#define BENCHMARK_SSS_HPP

#include "lib\supergrover\sigslot\src\signal.h"

#include "benchmark.hpp"

class Sss : public SigSlotBase
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Signal<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.bind(&Foo::handler, &foo);
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject.emit(rng);
    }
};

#endif // BENCHMARK_SSS_HPP
