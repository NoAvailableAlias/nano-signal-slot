#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <random>

// Friendly macro definitions
#if _MSC_VER >= 1400
#define NOINLINE(s) __declspec(noinline) s
#else
#define NOINLINE(s) s __attribute__ ((noinline))
#endif

// Globals
extern std::size_t g_limit;
const std::size_t c_jlsignal_max = 9001;

// General typedefs
typedef std::minstd_rand Rng_t;
typedef std::uniform_int_distribution<std::size_t> Eng_t;

// Unit related typedefs
typedef std::chrono::seconds Limit_u;
typedef std::chrono::nanoseconds Timer_u;
typedef std::chrono::duration<double, std::milli> Delta_u;

// Global Free Functions
std::size_t round_2_up(std::size_t n)
{
    if (n & (n - 1))
    {
        do
        {
            n = n & (n - 1);
        }
        while (n & (n - 1));

        return n << 1;
    }
    return n;
}
std::size_t round_2_down(std::size_t n)
{
    if (n & (n - 1))
    {
        do
        {
            n = n & (n - 1);
        }
        while (n & (n - 1));

        return n;
    }
    return n;
}

#endif // BENCHMARK_HPP
