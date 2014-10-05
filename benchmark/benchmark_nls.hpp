#ifndef BENCHMARK_NLS_HPP
#define BENCHMARK_NLS_HPP

#include "lib\neolib_sigslot\neolib.hpp"
#include "lib\neolib_sigslot\signal.hpp"
#include "lib\neolib_sigslot\slot.hpp"

#include "benchmark.hpp"

class Nls : public neolib::has_slots<>
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = neolib::signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject(foo, &Foo::handler);
    }
    template <typename Subject, typename Foo>
    static void emit_method(Subject& subject, Foo& rng)
    {
        subject.trigger(rng); // triggered
    }
};

#endif // BENCHMARK_NLS_HPP
