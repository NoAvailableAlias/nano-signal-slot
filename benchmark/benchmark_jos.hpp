#ifndef BENCHMARK_JOS_HPP
#define BENCHMARK_JOS_HPP

#include "lib/joanrieu/signal11/signal11.h"

#include "benchmark.hpp"

#include <forward_list>
#include <memory>

class Jos
{
    std::forward_list<std::shared_ptr<void>> reg;

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

        foo.reg.emplace_front(&foo, [con, &subject](void*)
        {
            subject.disconnect(con);
        });
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
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
