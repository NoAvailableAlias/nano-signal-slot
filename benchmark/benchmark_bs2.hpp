#ifndef BENCHMARK_BS2_HPP
#define BENCHMARK_BS2_HPP

//#define BOOST_BIND_NO_PLACEHOLDERS

#include <boost/signals2.hpp>
#include <boost/bind.hpp>

#include "benchmark.hpp"

class Bs2 : public boost::signals2::trackable
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = boost::signals2::signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.connect(boost::bind(&Foo::handler, &foo, ::_1));
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

#endif // BENCHMARK_BS2_HPP
