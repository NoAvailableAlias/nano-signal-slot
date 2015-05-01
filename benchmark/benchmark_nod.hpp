#ifndef BENCHMARK_NOD_HPP
#define BENCHMARK_NOD_HPP

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include "lib\nod\include\nod\nod.hpp"
#include "lib\nod\tests\test_helpers.hpp"

#include "benchmark.hpp"

#include <memory>

class Nod
{
    std::unique_ptr<nod::scoped_connection> reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = nod::signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        /*foo.reg = test::make_unique<nod::scoped_connection>(
            subject.connect([&foo](Rng& rng)
            {
                foo.handler(rng);
            })
        );*/
        foo.reg = test::make_unique<nod::scoped_connection>(
            subject.connect(std::bind(&Foo::handler, &foo, std::placeholders::_1))
        );
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

#endif // BENCHMARK_NOD_HPP
