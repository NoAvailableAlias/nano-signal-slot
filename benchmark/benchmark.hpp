#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <algorithm>
#include <cassert>
#include <chrono>
#include <memory>
#include <random>
#include <vector>
#include <tuple>

#ifdef _WIN32
#define NOINLINE(s) __declspec(noinline) s
#elif __unix__
#define NOINLINE(s) s __attribute__ ((noinline))
#else
#define NOINLINE(s) s
#endif

extern std::size_t g_limit;
const std::size_t c_jlsignal_max = 1024;

typedef std::minstd_rand Rng;

typedef std::chrono::nanoseconds Timer_u;
typedef std::chrono::milliseconds Limit_u;
typedef std::chrono::duration<double, std::milli> Delta_u;

//------------------------------------------------------------------------------

typedef std::shared_ptr<void> SlotScope;

template <typename Deleter>
SlotScope make_slot_scope(Deleter deleter)
{
    return SlotScope((void*)0xDEADC0DE, deleter);
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

struct IncrementFill
{
    std::size_t i = 0;
    std::size_t operator()() { return i++; }
};

//------------------------------------------------------------------------------

template <typename Subject, typename Foo> class Benchmark
{
    static std::vector<std::size_t> s_indices;
    static chrono_timer s_timer;
    static Rng s_rng;

    static double _calculate_score(std::size_t N, std::size_t limit, std::size_t count)
    {
        // Return the test size N divided by the average sample time in milliseconds
        return N / std::chrono::duration_cast<Delta_u>(Timer_u(limit / count)).count();
    }

    public:

    static void validation_assert(std::size_t N)
    {
        Rng rng;
        std::size_t count = 0;

        for (auto i = N; i != 0; --i, count += N)
        {
            Subject subject;
            std::vector<Foo> foo_array(N);

            for (auto& foo : foo_array)
            {
                Foo::connect_method(subject, foo);
            }
            Foo::emit_method(subject, rng);
        }
        // If the PRNG state is different
        // There is an error in the signal implementation
        Rng test;
        test.discard(count);
        assert(rng == test);
    }

    //--------------------------------------------------------------------------

    static double construction(std::size_t N)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < g_limit; ++count)
        {
            s_timer.reset();

            volatile Subject* subject = new Subject;
            std::vector<Foo> foo(N);

            elapsed += s_timer.count<Timer_u>();

            delete subject;
        }
        return _calculate_score(N, g_limit, count);
    }

    //--------------------------------------------------------------------------

    static double destruction(std::size_t N)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        s_indices.resize(N);
        std::generate(s_indices.begin(), s_indices.end(), IncrementFill());

        for (; elapsed < g_limit; ++count)
        {
            std::shuffle(s_indices.begin(), s_indices.end(), s_rng);
            {
                Subject subject;
                std::vector<Foo> foo(N);

                for (auto index : s_indices)
                {
                    Foo::connect_method(subject, foo[index]);
                }
                s_timer.reset();
                
                // Destruction
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return _calculate_score(N, g_limit, count);
    }

    //--------------------------------------------------------------------------

    static double connection(std::size_t N)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        s_indices.resize(N);
        std::generate(s_indices.begin(), s_indices.end(), IncrementFill());

        for (; elapsed < g_limit; ++count)
        {
            std::shuffle(s_indices.begin(), s_indices.end(), s_rng);

            Subject subject;
            std::vector<Foo> foo(N);

            s_timer.reset();

            for (auto index : s_indices)
            {
                Foo::connect_method(subject, foo[index]);
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return _calculate_score(N, g_limit, count);
    }

    //--------------------------------------------------------------------------

    static double emission(std::size_t N)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        s_indices.resize(N);
        std::generate(s_indices.begin(), s_indices.end(), IncrementFill());
        
        for (; elapsed < g_limit; ++count)
        {
            std::shuffle(s_indices.begin(), s_indices.end(), s_rng);

            Subject subject;
            std::vector<Foo> foo(N);

            for (auto index : s_indices)
            {
                Foo::connect_method(subject, foo[index]);
            }
            s_timer.reset();

            Foo::emit_method(subject, s_rng);

            elapsed += s_timer.count<Timer_u>();
        }
        return _calculate_score(N, g_limit, count);
    }

    //--------------------------------------------------------------------------

    static double combined(std::size_t N)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        s_indices.resize(N);
        std::generate(s_indices.begin(), s_indices.end(), IncrementFill());
        std::shuffle(s_indices.begin(), s_indices.end(), s_rng);

        s_timer.reset();
        
        for (; elapsed < g_limit; ++count, elapsed = s_timer.count<Timer_u>())
        {
            Subject subject;
            std::vector<Foo> foo(N);

            for (auto index : s_indices)
            {
                Foo::connect_method(subject, foo[index]);
            }
            Foo::emit_method(subject, s_rng);
        }
        return _calculate_score(N, g_limit, count);
    }
};

template <typename Subject, typename Foo>
std::vector<std::size_t> Benchmark<Subject, Foo>::s_indices;

template <typename Subject, typename Foo>
chrono_timer Benchmark<Subject, Foo>::s_timer;

template <typename Subject, typename Foo>
Rng Benchmark<Subject, Foo>::s_rng;

#endif // BENCHMARK_HPP
