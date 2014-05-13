#include "benchmark.hpp"

//#include "bs1_benchmark.hpp"
//#include "bs2_benchmark.hpp"

#include "evl_benchmark.hpp"
//#include "evf_benchmark.hpp"
#include "evs_benchmark.hpp"
#include "jls_benchmark.hpp"
#include "nss_benchmark.hpp"

#include "lib/jl_signal/Signal.h"
#include "lib/jl_signal/StaticSignalConnectionAllocators.h"

#include <unordered_map>
#include <iostream>
#include <numeric>
#include <vector>

std::size_t g_limit = Timer_u(Limit_u(4)).count();

int main(int argc, char* argv[])
{
    using namespace Benchmark;

    using std::cout;
    using std::endl;

    std::unordered_map<const char*, std::vector<double>> records;

    jl::StaticSignalConnectionAllocator<c_jlsignal_max> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<c_jlsignal_max> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    std::size_t ratios[] = { 75, 50, 25 };

    std::size_t start_n = round_2_down(8);
    std::size_t maximum_n = round_2_up(256);

    for (std::size_t N = start_n; N <= maximum_n; N *= 2)
    {
        for (auto R : ratios)
        {
            records["Nss"].emplace_back(Nss::combined(N, R));
        }
        for (auto R : ratios)
        {
            records["Evl"].emplace_back(Evl::combined(N, R));
        }
        for (auto R : ratios)
        {
            // Benchmark::Evf causing an internal compiler error
            //records["Evf"].emplace_back(Evf::combined(N, R));
        }
        for (auto R : ratios)
        {
            records["Evs"].emplace_back(Evs::combined(N, R));
        }
        for (auto R : ratios)
        {
            records["Jls"].emplace_back(Jls::combined(N, R));
        }
    }
    for (auto const& record : records)
    {
        std::vector<double> const& data = record.second;
        cout << record.first << " average relative performance: " <<
            std::accumulate(std::begin(data), std::end(data), 1.0) /
            (double) data.size() << endl;
    }
    std::cin.get();
}
