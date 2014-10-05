#ifndef BENCHMARK_PSG_HPP
#define BENCHMARK_PSG_HPP

#include "lib\pbhogan\Signals\Signal.h"

#include "benchmark.hpp"

#include <memory>
#include <vector>

class Psg
{
    std::vector<std::shared_ptr<void>> reg;

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

        foo.reg.emplace_back(&foo, [&](void*)
        {
            subject.Disconnect(&foo, &Foo::handler);
        });
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject.Emit(rng);
    }
};

#endif // BENCHMARK_PSG_HPP
