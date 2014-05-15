#include "benchmark.hpp"

#include "bs1_benchmark.hpp"
#include "bs2_benchmark.hpp"
#include "evl_benchmark.hpp"
#include "evf_benchmark.hpp"
#include "evs_benchmark.hpp"
#include "jls_benchmark.hpp"
#include "nss_benchmark.hpp"

#include "lib/jl_signal/Signal.h"
#include "lib/jl_signal/StaticSignalConnectionAllocators.h"

#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <vector>

// default to 4 seconds because 32 bit
std::size_t g_limit = Timer_u(Limit_u(4)).count();

int main(int argc, char* argv[])
{
    using namespace Benchmark;

    jl::StaticSignalConnectionAllocator<c_jlsignal_max> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<c_jlsignal_max> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    std::unordered_map<const char*, std::vector<double>> records;

    std::size_t start_n = round_2_down(8);
    std::size_t maximum_n = round_2_up(128);

    try
    {
        for (std::size_t N = start_n; N <= maximum_n; N *= 2)
        {
            for (auto R : { 75, 50, 25 }) // emission ratio
            {
                records["Boost Signals"].emplace_back(Bs1::combined(N, R));
                records["Boost Signals2"].emplace_back(Bs2::combined(N, R));
                records["Nano-signal-slot"].emplace_back(Nss::combined(N, R));
                records["EvilTwin Observer"].emplace_back(Evl::combined(N, R));
                records["EvilTwin Thread Safe"].emplace_back(Evs::combined(N, R));
                records["EvilTwin Fork"].emplace_back(Evf::combined(N, R));
                records["Jl_signal"].emplace_back(Jls::combined(N, R));
            }
        }
        for (auto const& record : records)
        {
            std::vector<double> const& data = record.second;
            std::cout << std::setw(25) << std::left << record.first
                << std::right << " [higher score is better]: "
                << std::accumulate(std::begin(data), std::end(data), 1.0)
                / (double) data.size() << std::endl;
        }
    }
    catch (std::exception const& error)
    {
        std::cerr << error.what() << std::endl;
    }
    std::cin.get();
}
