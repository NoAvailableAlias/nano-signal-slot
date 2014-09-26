#ifndef BS1_BENCHMARK_HPP
#define BS1_BENCHMARK_HPP

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include "benchmark.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace Benchmark
{

class Bs1 : public boost::signals::trackable
{
    NOINLINE(void handler(Rng_t& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    using Subject = boost::signal<void(Rng_t&)>;
    using Foo = Benchmark::Bs1;

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
                Subject subject;
                std::vector<Foo> foo_array(N);

                for (auto index : randomized)
                {
                    auto& foo = foo_array[index];
                    subject.connect(boost::bind(&Foo::handler, &foo, _1));
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
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
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
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
            }
            s_timer.reset();
            subject(rng);
            elapsed += s_timer.count<Timer_u>();
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static double combined(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;
        std::size_t elapsed = 0;
        const std::size_t limit = g_limit;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        s_timer.reset();

        for (; elapsed < limit; ++count, elapsed = s_timer.count<Timer_u>())
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
            }
            subject(rng);
        }
        return testsize_over_dt(N, limit, count);
    }

//------------------------------------------------------------------------------

    NOINLINE(static std::size_t validate(std::size_t N))
    {
        Rng_t rng;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());

        for (auto i = N; i != 0; --i)
        {
            std::shuffle(randomized.begin(), randomized.end(), rng);

            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
            }
            subject(rng);
        }
        return rng();
    }
};

chrono_timer Bs1::s_timer;

} // namespace Benchmark -------------------------------------------------------

#endif // BS1_BENCHMARK_HPP
