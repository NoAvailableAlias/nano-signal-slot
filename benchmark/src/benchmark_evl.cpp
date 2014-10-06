#include "../benchmark_evl.hpp"

NOINLINE(void Evl::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Evl>::validation_assert(N);
}    
NOINLINE(double Evl::construction(std::size_t N))
{
    return Benchmark<Signal, Evl>::construction(N);
}
NOINLINE(double Evl::destruction(std::size_t N))
{
    return Benchmark<Signal, Evl>::destruction(N);
}
NOINLINE(double Evl::connection(std::size_t N))
{
    return Benchmark<Signal, Evl>::connection(N);
}
NOINLINE(double Evl::emission(std::size_t N))
{
    return Benchmark<Signal, Evl>::emission(N);
}
NOINLINE(double Evl::combined(std::size_t N))
{
    return Benchmark<Signal, Evl>::combined(N);
}
