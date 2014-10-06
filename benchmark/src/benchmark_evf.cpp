#include "../benchmark_evf.hpp"

NOINLINE(void Evf::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Evf>::validation_assert(N);
}    
NOINLINE(double Evf::construction(std::size_t N))
{
    return Benchmark<Signal, Evf>::construction(N);
}
NOINLINE(double Evf::destruction(std::size_t N))
{
    return Benchmark<Signal, Evf>::destruction(N);
}
NOINLINE(double Evf::connection(std::size_t N))
{
    return Benchmark<Signal, Evf>::connection(N);
}
NOINLINE(double Evf::emission(std::size_t N))
{
    return Benchmark<Signal, Evf>::emission(N);
}
NOINLINE(double Evf::combined(std::size_t N))
{
    return Benchmark<Signal, Evf>::combined(N);
}
