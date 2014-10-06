#include "../benchmark_nls.hpp"

NOINLINE(void Nls::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Nls>::validation_assert(N);
}    
NOINLINE(double Nls::construction(std::size_t N))
{
    return Benchmark<Signal, Nls>::construction(N);
}
NOINLINE(double Nls::destruction(std::size_t N))
{
    return Benchmark<Signal, Nls>::destruction(N);
}
NOINLINE(double Nls::connection(std::size_t N))
{
    return Benchmark<Signal, Nls>::connection(N);
}
NOINLINE(double Nls::emission(std::size_t N))
{
    return Benchmark<Signal, Nls>::emission(N);
}
NOINLINE(double Nls::combined(std::size_t N))
{
    return Benchmark<Signal, Nls>::combined(N);
}
