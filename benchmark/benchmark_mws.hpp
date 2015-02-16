#ifndef BENCHMARK_MWS_HPP
#define BENCHMARK_MWS_HPP

#include "lib/mwthinker/Signal/src/mw/signal.h"

#include "benchmark.hpp"

class Mws
{
    mw::signals::Connection reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = mw::Signal<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        foo.reg = subject.connect(std::bind(&Foo::handler, &foo, std::placeholders::_1));
    }
    template <typename Subject, typename Arg>
    static void emit_method(Subject& subject, Arg& rng)
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

#endif // BENCHMARK_MWS_HPP
