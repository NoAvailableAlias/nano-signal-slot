#include "../benchmark_nod.hpp"

NOINLINE(void Nod::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Nod>::validation_assert(N);
}    
NOINLINE(double Nod::construction(std::size_t N))
{
    return Benchmark<Signal, Nod>::construction(N);
}
NOINLINE(double Nod::destruction(std::size_t N))
{
    return Benchmark<Signal, Nod>::destruction(N);
}
NOINLINE(double Nod::connection(std::size_t N))
{
    return Benchmark<Signal, Nod>::connection(N);
}
NOINLINE(double Nod::emission(std::size_t N))
{
    return Benchmark<Signal, Nod>::emission(N);
}
NOINLINE(double Nod::combined(std::size_t N))
{
    return Benchmark<Signal, Nod>::combined(N);
}
