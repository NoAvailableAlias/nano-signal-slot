#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <random>
#include <tuple>

// "Friendly" macro definitions
#if _MSC_VER >= 1400
#define NOINLINE(s) __declspec(noinline) s
#else
#define NOINLINE(s) s __attribute__ ((noinline))
#endif

// Globals
extern std::size_t g_limit;
const std::size_t c_jlsignal_max = 1024;

// General typedefs
typedef std::minstd_rand Rng_t;
typedef std::uniform_int_distribution<std::size_t> Eng_t;

// Unit related typedefs
typedef std::chrono::seconds Limit_u;
typedef std::chrono::nanoseconds Timer_u;
typedef std::chrono::duration<double, std::milli> Delta_u;

// Functors
struct IncrementFill
{
    std::size_t i;
    IncrementFill ( ) : i(0) { }
    std::size_t operator()() { return i++; }
};

//------------------------------------------------------------------------------

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

inline double testsize_over_dt(std::size_t N, std::size_t limit, std::size_t count)
{
    return N / std::chrono::duration_cast<Delta_u>(Timer_u(limit / count)).count();
}

//------------------------------------------------------------------------------

// based stackoverflow

template <unsigned int N>
struct tee_stream
{
    template <typename ...Args, typename T>
    static std::tuple<Args...>& print(std::tuple<Args...>& t, T&& x)
    {
        std::get<sizeof...(Args) - N>(t) << x;
        tee_stream<N - 1>::print(t, std::forward<T>(x));
        return t;
    }
};

template <>
struct tee_stream<0>
{
    template <typename ...Args, typename T>
    static std::tuple<Args...>& print(std::tuple<Args...>& t, T&& x)
    {
        return t;
    }
};

template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>& t, T&& x)
{
    return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}

template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>&& t, T&& x)
{
    return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}

#endif // BENCHMARK_HPP
