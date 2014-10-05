#ifndef BENCHMARK_EVF_HPP
#define BENCHMARK_EVF_HPP

#include "lib/eviltwin/observer_fork.hpp"

#include "benchmark.hpp"

class Evf
{
    std::forward_list<Nano::Fork::UniversalPtr> reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Nano::Fork::Subject<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        foo.reg.emplace_front(subject.connect(
            std::bind(&Foo::handler, &foo, std::placeholders::_1)));
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject(rng);
    }
};

#endif // BENCHMARK_EVF_HPP
