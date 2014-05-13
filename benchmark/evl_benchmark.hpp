#ifndef EVL_BENCHMARK_HPP
#define EVL_BENCHMARK_HPP

#include "lib/eviltwin/observer.hpp"

#include "chrono_timer.hpp"
#include "benchmark.hpp"

#include <forward_list>
#include <vector>

namespace Benchmark
{

class Evl
{
    NOINLINE(void handler(Rng_t& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    using Subject = obs::Subject<void(Rng_t&)>;
    using Foo = Benchmark::Evl;

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

        using std::placeholders::_1;

        for (; g_limit > timer.count<Timer_u>(); ++count)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            std::forward_list<obs::detail::UniversalPtr> reg;

            for (auto& foo : foo_array)
            {
                if (eng(rng) > ratio)
                {
                    reg.emplace_front(subject.registerObserver
                        (std::bind(&Foo::handler, &foo, _1)));
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

chrono_timer Evl::timer;

} // namespace Benchmark -------------------------------------------------------

#endif // EVL_BENCHMARK_HPP
