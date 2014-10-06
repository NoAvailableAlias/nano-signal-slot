#include "../benchmark_bs2.hpp"

NOINLINE(void Bs2::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Bs2>::validation_assert(N);
}    
NOINLINE(double Bs2::construction(std::size_t N))
{
    return Benchmark<Signal, Bs2>::construction(N);
}
NOINLINE(double Bs2::destruction(std::size_t N))
{
    return Benchmark<Signal, Bs2>::destruction(N);
}
NOINLINE(double Bs2::connection(std::size_t N))
{
    return Benchmark<Signal, Bs2>::connection(N);
}
NOINLINE(double Bs2::emission(std::size_t N))
{
    return Benchmark<Signal, Bs2>::emission(N);
}
NOINLINE(double Bs2::combined(std::size_t N))
{
    return Benchmark<Signal, Bs2>::combined(N);
}
