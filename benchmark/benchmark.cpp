#include "benchmark.hpp"

#include "bs1_benchmark.hpp"
#include "bs2_benchmark.hpp"
#include "evl_benchmark.hpp"
#include "evs_benchmark.hpp"
#include "jls_benchmark.hpp"
#include "nss_benchmark.hpp"

#include "lib/jl_signal/Signal.h"
#include "lib/jl_signal/StaticSignalConnectionAllocators.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <vector>
#include <map>

using Table = std::map<const char*, std::vector<double>>;
using DataBase = std::map<const char*, Table>;

std::size_t g_limit = Timer_u(Limit_u(4)).count();
//std::size_t g_limit = 10000000;

//------------------------------------------------------------------------------

void outputReport(DataBase const& records, std::ostream& ost)
{
    for (auto const& table : records)
    {
        auto tableName = table.first;

        ost << "\n    " << tableName << ":";

        for (auto const& record : table.second)
        {
            auto recordName = record.first;
            auto data = record.second;

            ost << "\n ++ " << std::setw(40) << std::setfill('_')
                << recordName << ": " << std::setprecision(2) << std::fixed
                << std::accumulate(std::begin(data), std::end(data), 1.0)
                / (double) data.size();
        }
        ost << std::endl;
    }
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    using namespace Benchmark;

    jl::StaticSignalConnectionAllocator<c_jlsignal_max> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<c_jlsignal_max> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    DataBase records;

    std::size_t start_n = round_2_down(8);
    std::size_t maximum_n = round_2_up(64);

    for (std::size_t N = start_n; N <= maximum_n; N *= 2)
    {
        std::cout << N << ", " << __LINE__ << std::endl;

        auto& bs1 = records["Boost Signals"];
        bs1["construction"].emplace_back(Bs1::construction(N));
        bs1["destruction"].emplace_back(Bs1::destruction(N));
        bs1["connection"].emplace_back(Bs1::connection(N));
        bs1["emission"].emplace_back(Bs1::emission(N));
        bs1["combined"].emplace_back(Bs1::combined(N));

        std::cout << N << ", " << __LINE__ << std::endl;

        auto& bs2 = records["Boost Signals2"];
        bs2["construction"].emplace_back(Bs2::construction(N));
        bs2["destruction"].emplace_back(Bs2::destruction(N));
        bs2["connection"].emplace_back(Bs2::connection(N));
        bs2["emission"].emplace_back(Bs2::emission(N));
        bs2["combined"].emplace_back(Bs2::combined(N));
        
        std::cout << N << ", " << __LINE__ << std::endl;

        auto& nss = records["Nano-signal-slot"];
        nss["construction"].emplace_back(Nss::construction(N));
        nss["destruction"].emplace_back(Nss::destruction(N));
        nss["connection"].emplace_back(Nss::connection(N));
        nss["emission"].emplace_back(Nss::emission(N));
        nss["combined"].emplace_back(Nss::combined(N));
        
        std::cout << N << ", " << __LINE__ << std::endl;

        auto& evl = records["EvilTwin Observer"];
        evl["construction"].emplace_back(Evl::construction(N));
        evl["destruction"].emplace_back(Evl::destruction(N));
        evl["connection"].emplace_back(Evl::connection(N));
        evl["emission"].emplace_back(Evl::emission(N));
        evl["combined"].emplace_back(Evl::combined(N));
        
        std::cout << N << ", " << __LINE__ << std::endl;

        auto& evs = records["EvilTwin Thread Safe"];
        evs["construction"].emplace_back(Evs::construction(N));
        evs["destruction"].emplace_back(Evs::destruction(N));
        evs["connection"].emplace_back(Evs::connection(N));
        evs["emission"].emplace_back(Evs::emission(N));
        evs["combined"].emplace_back(Evs::combined(N));
        
        std::cout << N << ", " << __LINE__ << std::endl;

        auto& jls = records["Jl_signal"];
        jls["construction"].emplace_back(Jls::construction(N));
        jls["destruction"].emplace_back(Jls::destruction(N));
        jls["connection"].emplace_back(Jls::connection(N));
        jls["emission"].emplace_back(Jls::emission(N));
        jls["combined"].emplace_back(Jls::combined(N));
    }
    if (auto ofs = std::ofstream("report.txt", std::ios::app))
    {
        outputReport(records, ofs);
        outputReport(records, std::cout);

        ofs << "\n//--------------------------------------"
            << "----------------------------------------\n" << std::endl;
    }
    else
    {
        std::cerr << "ofstream error" << std::endl;
    }
    std::cin.get();
}
