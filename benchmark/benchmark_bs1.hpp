#ifndef BENCHMARK_BS1_HPP
#define BENCHMARK_BS1_HPP

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include "benchmark.hpp"

class Bs1 : public boost::signals::trackable
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = boost::signal<void(Rng&)>;

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
};

#endif // BENCHMARK_BS1_HPP
