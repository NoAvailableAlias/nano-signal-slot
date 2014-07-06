#ifndef BS1_BENCHMARK_HPP
#define BS1_BENCHMARK_HPP

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include "chrono_timer.hpp"
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

    static chrono_timer timer;

    public:

//------------------------------------------------------------------------------

    NOINLINE(static double construction(std::size_t N))
    {
        std::size_t count = 1, elapsed = 0;

        for (; elapsed < g_limit; ++count)
        {
            timer.reset();
            std::unique_ptr<Subject> subject(new Subject);
            std::vector<Foo> foo_array(N);
            subject->connect(boost::bind(&Foo::handler, &foo_array.back(), _1));
            elapsed += timer.count<Timer_u>();
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }

//------------------------------------------------------------------------------

    NOINLINE(static double destruction(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1, elapsed = 0;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        for (; elapsed < g_limit; ++count)
        {
            {
                std::unique_ptr<Subject> subject(new Subject);
                std::vector<Foo> foo_array(N);

                for (auto index : randomized)
                {
                    auto& foo = foo_array[index];
                    subject->connect(boost::bind(&Foo::handler, &foo, _1));
                }
                timer.reset();
            }
            elapsed += timer.count<Timer_u>();
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }

//------------------------------------------------------------------------------

    NOINLINE(static double connection(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1, elapsed = 0;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        for (; elapsed < g_limit; ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            timer.reset();
            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
            }
            elapsed += timer.count<Timer_u>();
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }

//------------------------------------------------------------------------------

    NOINLINE(static double emission(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1, elapsed = 0;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        for (; elapsed < g_limit; ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto index : randomized)
            {
                auto& foo = foo_array[index];
                subject.connect(boost::bind(&Foo::handler, &foo, _1));
            }
            timer.reset();
            subject(rng);
            elapsed += timer.count<Timer_u>();
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }

//------------------------------------------------------------------------------

    NOINLINE(static double combined(std::size_t N))
    {
        Rng_t rng;
        std::size_t count = 1;

        std::vector<std::size_t> randomized(N);
        std::generate(randomized.begin(), randomized.end(), IncrementFill());
        std::shuffle(randomized.begin(), randomized.end(), rng);

        timer.reset();

        for (; g_limit > timer.count<Timer_u>(); ++count)
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
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }
};

chrono_timer Bs1::timer;

} // namespace Benchmark -------------------------------------------------------

#endif // BS1_BENCHMARK_HPP
