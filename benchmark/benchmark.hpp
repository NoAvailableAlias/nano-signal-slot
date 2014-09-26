#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <random>
#include <tuple>

// "Friendly" macro definitions
#ifdef _WIN32
#define NOINLINE(s) __declspec(noinline) s
#elif __unix__
#define NOINLINE(s) s __attribute__ ((noinline))
#else
#define NOINLINE(s) s
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
    std::size_t i = 0;
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

//------------------------------------------------------------------------------

template <typename Iterator> struct RangeHelper
{
    Iterator lhs;
    Iterator rhs;

    Iterator cbegin() const { return lhs; }
    Iterator begin() const { return lhs; }
    Iterator cend() const { return rhs; }
    Iterator end() const { return rhs; }
};
template <typename Iterator>
static RangeHelper<Iterator> Range(Iterator lhs,
                                   Iterator rhs)
{
    return { lhs, rhs };
}

//------------------------------------------------------------------------------

class chrono_timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    public:

    void reset()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }
    template <typename T>
    std::size_t count() const
    {
        return std::chrono::duration_cast<T>
          (std::chrono::high_resolution_clock::now() - m_start).count();
    }
};

#endif // BENCHMARK_HPP
