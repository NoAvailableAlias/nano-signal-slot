#ifndef S11_BENCHMARK_HPP
#define S11_BENCHMARK_HPP

#include "lib/signal11/Signal.h"

#include "chrono_timer.hpp"
#include "benchmark.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace Benchmark
{

class S11
{
    Signal11::ConnectionScope reg;

    NOINLINE(void handler(Rng_t& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    using Subject = Signal11::Signal<void(Rng_t&)>;
    using Foo = Benchmark::S11;

    static chrono_timer s_timer;

    public:

//------------------------------------------------------------------------------

    NOINLINE(static double construction(std::size_t N))
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;
        const std::size_t limit = g_limit;

        for (; elapsed < limit; ++count)
        {
            s_timer.reset();

            std::unique_ptr<Subject> subject(new Subject);
            std::vector<Foo> foo_array(N);

            elapsed += s_timer.count<Timer_u>();
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static double destruction(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;
        std::size_t elapsed = 0;
        const std::size_t limit = g_limit;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());

        for (; elapsed < limit; ++count)
        {
            std::shuffle(randomized.begin(), randomized.end(), rng);
            {
                std::unique_ptr<Subject> subject(new Subject);
                std::vector<Foo> foo_array(N);

                for (auto index : randomized)
                {
                    auto& foo = foo_array[index];
                    subject->connect(&foo, &Foo::handler);
                    foo.reg += subject->connect(&foo, &Foo::handler);
                }
                s_timer.reset();
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static double connection(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;
        std::size_t elapsed = 0;
        const std::size_t limit = g_limit;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());

        for (; elapsed < limit; ++count)
        {
            std::shuffle(randomized.begin(), randomized.end(), rng);

            Subject subject;
            std::vector<Foo> foo_array(N);

            s_timer.reset();
            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                foo.reg += subject.connect(&foo, &Foo::handler);
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static double emission(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;
        std::size_t elapsed = 0;
        const std::size_t limit = g_limit;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());

        for (; elapsed < limit; ++count)
        {
            std::shuffle(randomized.begin(), randomized.end(), rng);

            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                foo.reg += subject.connect(&foo, &Foo::handler);
            }
            s_timer.reset();
            subject.emit(rng);
            elapsed += s_timer.count<Timer_u>();
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static double combined(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;
        const std::size_t limit = g_limit;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        s_timer.reset();

        for (; s_timer.count<Timer_u>() < limit; ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                foo.reg += subject.connect(&foo, &Foo::handler);
            }
            subject.emit(rng);
        }
        return testsize_over_dt(N, limit, count);
    }
};

chrono_timer S11::s_timer;

} // namespace Benchmark -------------------------------------------------------

#endif // S11_BENCHMARK_HPP
