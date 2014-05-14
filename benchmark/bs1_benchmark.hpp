#ifndef BS1_BENCHMARK_HPP
#define BS1_BENCHMARK_HPP

#include <boost/signals.hpp>
#include <boost/bind.hpp>

#include "chrono_timer.hpp"
#include "benchmark.hpp"

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
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double destruction(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double connection(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double emission(std::size_t N))
    {
        std::size_t count = 0;
        return 1.0;
    }

//------------------------------------------------------------------------------

    NOINLINE(static double combined(std::size_t N, std::size_t ratio))
    {
        Rng_t rng;
        Eng_t eng(0, 100);
        std::size_t count = 0;
        timer.reset();

        for (; g_limit > timer.count<Timer_u>(); ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto& foo : foo_array)
            {
                if (eng(rng) > ratio)
                {
                    subject.connect(boost::bind(&Foo::handler, &foo, _1));
                }
                else
                {
                    subject(rng);
                }
            }
        }
        return N / std::chrono::duration_cast<Delta_u>
            (Timer_u(g_limit / count)).count();
    }
};

chrono_timer Bs1::timer;

} // namespace Benchmark -------------------------------------------------------

#endif // BS1_BENCHMARK_HPP
