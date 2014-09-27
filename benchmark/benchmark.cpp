#include "benchmark.hpp"

#include "asg_benchmark.hpp"
#include "bs1_benchmark.hpp"
#include "bs2_benchmark.hpp"
#include "evf_benchmark.hpp"
#include "evl_benchmark.hpp"
#include "jls_benchmark.hpp"
#include "nls_benchmark.hpp"
#include "nss_benchmark.hpp"
#include "psg_benchmark.hpp"
#include "sss_benchmark.hpp"
#include "wsg_benchmark.hpp"

#include "lib/jl_signal/Signal.h"
#include "lib/jl_signal/StaticSignalConnectionAllocators.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <map>

const char* construction = "construct";
const char* destruction = "destruct";
const char* connection = "connect";
const char* emission = "emission";
const char* combined = "combined";

using ImmediateResults = std::map<const char*, std::vector<double>>;
using ImmediateData = std::map<const char*, ImmediateResults>;

std::size_t g_limit = Timer_u(Limit_u(2)).count();
//std::size_t g_limit = 100000000;
//std::size_t g_limit = 10000000;
//std::size_t g_limit = 1000000;

template <typename T> void outputReport(ImmediateData const&, T&);

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    using namespace Benchmark;

    jl::StaticSignalConnectionAllocator<c_jlsignal_max> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<c_jlsignal_max> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    ImmediateData records;

    std::size_t start_n = round_2_down(8);
    std::size_t maximum_n = round_2_up(64);

    // Make sure to set process to High priority
    // Make sure to set process cpu affinity to only one core
    std::cout << "Change CPU priority now: [paused]" << std::endl;
    std::cin.get();

    // Make sure each implementation functions correctly
    // Prng state should return the same state for all tests
    std::cout << Asg::validate(maximum_n) << " amc522 Signal11" << std::endl;
    std::cout << Bs1::validate(maximum_n) << " Boost Signals" << std::endl;
    std::cout << Bs2::validate(maximum_n) << " Boost Signals2" << std::endl;
    std::cout << Evl::validate(maximum_n) << " EvilTwin Observer" << std::endl;
    std::cout << Evf::validate(maximum_n) << " EvilTwin Observer Fork" << std::endl;
    std::cout << Jls::validate(maximum_n) << " Jl_signal" << std::endl;
    std::cout << Nls::validate(maximum_n) << " neosigslot" << std::endl;
    std::cout << Nss::validate(maximum_n) << " Nano-signal-slot" << std::endl;
    std::cout << Psg::validate(maximum_n) << " pbhogan Signals" << std::endl;
    std::cout << Sss::validate(maximum_n) << " supergrover sigslot" << std::endl;
    std::cout << Wsg::validate(maximum_n) << " winglot Signals\n" << std::endl;

    auto start = std::chrono::system_clock::now();
    auto start_c = std::chrono::system_clock::to_time_t(start);

    // Double the input size N for every iteration
    // Timing is done in nanoseconds then converted to milliseconds
    for(std::size_t N = start_n; N <= maximum_n; N *= 2)
    {
        std::cout << "[Test Size: " << N << "] BEGIN\n" << std::endl;

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& asg = records["amc522 Signal11"];
        asg[construction].emplace_back(Asg::construction(N));
        asg[destruction].emplace_back(Asg::destruction(N));
        asg[connection].emplace_back(Asg::connection(N));
        asg[emission].emplace_back(Asg::emission(N));
        asg[combined].emplace_back(Asg::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& bs1 = records["Boost Signals"];
        bs1[construction].emplace_back(Bs1::construction(N));
        bs1[destruction].emplace_back(Bs1::destruction(N));
        bs1[connection].emplace_back(Bs1::connection(N));
        bs1[emission].emplace_back(Bs1::emission(N));
        bs1[combined].emplace_back(Bs1::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& bs2 = records["Boost Signals2"];
        bs2[construction].emplace_back(Bs2::construction(N));
        bs2[destruction].emplace_back(Bs2::destruction(N));
        bs2[connection].emplace_back(Bs2::connection(N));
        bs2[emission].emplace_back(Bs2::emission(N));
        bs2[combined].emplace_back(Bs2::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& evl = records["EvilTwin Observer"];
        evl[construction].emplace_back(Evl::construction(N));
        evl[destruction].emplace_back(Evl::destruction(N));
        evl[connection].emplace_back(Evl::connection(N));
        evl[emission].emplace_back(Evl::emission(N));
        evl[combined].emplace_back(Evl::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& evf = records["EvilTwin Obs Fork"];
        evf[construction].emplace_back(Evf::construction(N));
        evf[destruction].emplace_back(Evf::destruction(N));
        evf[connection].emplace_back(Evf::connection(N));
        evf[emission].emplace_back(Evf::emission(N));
        evf[combined].emplace_back(Evf::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& jls = records["Jl_signal"];
        jls[construction].emplace_back(Jls::construction(N));
        jls[destruction].emplace_back(Jls::destruction(N));
        jls[connection].emplace_back(Jls::connection(N));
        jls[emission].emplace_back(Jls::emission(N));
        jls[combined].emplace_back(Jls::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nls = records["neosigslot"];
        nls[construction].emplace_back(Nls::construction(N));
        nls[destruction].emplace_back(Nls::destruction(N));
        nls[connection].emplace_back(Nls::connection(N));
        nls[emission].emplace_back(Nls::emission(N));
        nls[combined].emplace_back(Nls::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nss = records["Nano-signal-slot"];
        nss[construction].emplace_back(Nss::construction(N));
        nss[destruction].emplace_back(Nss::destruction(N));
        nss[connection].emplace_back(Nss::connection(N));
        nss[emission].emplace_back(Nss::emission(N));
        nss[combined].emplace_back(Nss::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& psg = records["pbhogan Signals"];
        psg[construction].emplace_back(Psg::construction(N));
        psg[destruction].emplace_back(Psg::destruction(N));
        psg[connection].emplace_back(Psg::connection(N));
        psg[emission].emplace_back(Psg::emission(N));
        psg[combined].emplace_back(Psg::combined(N));
        
        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& sss = records["supergrover sigslot"];
        sss[construction].emplace_back(Sss::construction(N));
        sss[destruction].emplace_back(Sss::destruction(N));
        sss[connection].emplace_back(Sss::connection(N));
        sss[emission].emplace_back(Sss::emission(N));
        sss[combined].emplace_back(Sss::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& wsg = records["winglot Signals"];
        wsg[construction].emplace_back(Wsg::construction(N));
        wsg[destruction].emplace_back(Wsg::destruction(N));
        wsg[connection].emplace_back(Wsg::connection(N));
        wsg[emission].emplace_back(Wsg::emission(N));
        wsg[combined].emplace_back(Wsg::combined(N));

        std::cout << "\n[Test Size: " << N << "] END" << std::endl;
    }

    if (std::ofstream ofs { "report.txt", std::ios::app })
    {
        auto tee = std::tie(std::cout, ofs);
        tee << "\n" << std::put_time(std::localtime(&start_c), "%c") << "\n";

        outputReport(records, tee);

        auto stop = std::chrono::system_clock::now();
        auto stop_c = std::chrono::system_clock::to_time_t(stop);
        tee << "\n" << std::put_time(std::localtime(&stop_c), "%c") << "\n";

        ofs << std::endl;
    }
    else
    {
        std::cerr << "Unable to append report.txt: [error]" << std::endl;
    }
    std::cout << "\nBenchmarks Completed: [paused]" << std::endl;
    std::cin.get();
}

//------------------------------------------------------------------------------

template <typename T>
void outputReport(ImmediateData const& records, T& ost)
{
    using namespace std;

    using RelativeResults = map<const char*, double>;
    using RelativeData = map<const char*, RelativeResults>;
    using OrderedResults = pair<const char*, RelativeResults*>;
    using OrderedData = map<double, OrderedResults>;

    RelativeData resultAverage;
    OrderedData resultOrder;

    // Process and sort results by total score (sum of column averages)

    for(auto const& row : records)
    {
        auto const& libName = row.first;

        double score = 0.0;

        for(auto const& column : row.second)
        {
            auto const& opName = column.first;
            auto const& val = column.second;

            double average = accumulate(begin(val),
                end(val), 1.0) / (double) val.size();

            resultAverage[libName][opName] = average;
            score += average;
        }
        resultOrder[score] = OrderedResults { libName, &resultAverage[libName] };
    }

    // Print output in cvs format

    ost << "\nLibrary, " << construction << ", " << destruction << ", "
        << connection << ", " << emission << ", " << combined << "\n";

    for(auto const& row : Range(resultOrder.rbegin(), resultOrder.rend()))
    {
        auto const& score = row.first;
        auto const& libName = row.second.first;

        ost << libName;

        for(auto const& column : (*row.second.second))
        {
            auto const& opName = column.first;
            auto const& val = column.second;

            ost << ", " << setprecision(2) << fixed << val;
        }
        ost << "\n";
    }
}
