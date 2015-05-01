#include "benchmark_asg.hpp"
#include "benchmark_bs1.hpp"
#include "benchmark_bs2.hpp"
#include "benchmark_evf.hpp"
#include "benchmark_evl.hpp"
#include "benchmark_jls.hpp"
#include "benchmark_jos.hpp"
#include "benchmark_mws.hpp"
#include "benchmark_nls.hpp"
#include "benchmark_nod.hpp"
#include "benchmark_nss.hpp"
#include "benchmark_psg.hpp"
#include "benchmark_sss.hpp"
#include "benchmark_wsg.hpp"
#include "benchmark_yas.hpp"

#include "lib/jeffomatic/jl_signal/src/Signal.h"
#include "lib/jeffomatic/jl_signal/src/StaticSignalConnectionAllocators.h"

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

std::size_t g_limit = Timer_u(Limit_u(4000)).count();

template <typename T> void outputReport(ImmediateData const&, T&);

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Jl_signal uses a static allocator for high performance
    jl::StaticSignalConnectionAllocator<c_jlsignal_max> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<c_jlsignal_max> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    ImmediateData records;

    std::size_t start_n = 4;
    std::size_t maximum_n = 64;

    // 32 bit limit is ~ 4294 ms (when converted to nanoseconds)
    // Total duration at 4294 is around ~8 hours depending on hardware
    std::cout << "Enter the time limit per sample [milliseconds]: ";
    std::size_t limit;

    if (!(std::cin >> limit))
    {
        std::cerr << "Invalid time limit entered!" << std::endl;
        return 1;
    }
    g_limit = Timer_u(Limit_u(limit)).count();
    std::cin.ignore();

    // Make sure to set process to high priority and affinity to 1 core
    std::cout << "Change the CPU priority now: [paused]" << std::endl;
    std::cin.get();

    // Assert that all implementations are functioning correctly
    Asg::validate_assert(maximum_n);
    Bs1::validate_assert(maximum_n);
    Bs2::validate_assert(maximum_n);
    Evf::validate_assert(maximum_n);
    Evl::validate_assert(maximum_n);
    Jls::validate_assert(maximum_n);
    Jos::validate_assert(maximum_n);
    Mws::validate_assert(maximum_n);
    Nls::validate_assert(maximum_n);
    Nod::validate_assert(maximum_n);
    Nss::validate_assert(maximum_n);
    Psg::validate_assert(maximum_n);
    Sss::validate_assert(maximum_n);
    Wsg::validate_assert(maximum_n);
    Yas::validate_assert(maximum_n);

    // Time the entire duration of the benchmark
    auto start = std::chrono::system_clock::now();
    auto start_c = std::chrono::system_clock::to_time_t(start);

    // Double the input size N for every iteration
    for(std::size_t N = start_n; N <= maximum_n; N *= 2)
    {
        std::cout << "[Test Size: " << N << "] BEGIN\n" << std::endl;

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& asg = records["amc522 Signal11"];
        asg[construction].push_back(Asg::construction(N));
        asg[destruction].push_back(Asg::destruction(N));
        asg[connection].push_back(Asg::connection(N));
        asg[emission].push_back(Asg::emission(N));
        asg[combined].push_back(Asg::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& bs1 = records["Boost Signals"];
        bs1[construction].push_back(Bs1::construction(N));
        bs1[destruction].push_back(Bs1::destruction(N));
        bs1[connection].push_back(Bs1::connection(N));
        bs1[emission].push_back(Bs1::emission(N));
        bs1[combined].push_back(Bs1::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& bs2 = records["* Boost Signals2"];
        bs2[construction].push_back(Bs2::construction(N));
        bs2[destruction].push_back(Bs2::destruction(N));
        bs2[connection].push_back(Bs2::connection(N));
        bs2[emission].push_back(Bs2::emission(N));
        bs2[combined].push_back(Bs2::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& evf = records["EvilTwin Fork"];
        evf[construction].push_back(Evf::construction(N));
        evf[destruction].push_back(Evf::destruction(N));
        evf[connection].push_back(Evf::connection(N));
        evf[emission].push_back(Evf::emission(N));
        evf[combined].push_back(Evf::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& evl = records["EvilTwin Observer"];
        evl[construction].push_back(Evl::construction(N));
        evl[destruction].push_back(Evl::destruction(N));
        evl[connection].push_back(Evl::connection(N));
        evl[emission].push_back(Evl::emission(N));
        evl[combined].push_back(Evl::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& jls = records["jeffomatic jl_signal"];
        jls[construction].push_back(Jls::construction(N));
        jls[destruction].push_back(Jls::destruction(N));
        jls[connection].push_back(Jls::connection(N));
        jls[emission].push_back(Jls::emission(N));
        jls[combined].push_back(Jls::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& jos = records["joanrieu signal11"];
        jos[construction].push_back(Jos::construction(N));
        jos[destruction].push_back(Jos::destruction(N));
        jos[connection].push_back(Jos::connection(N));
        jos[emission].push_back(Jos::emission(N));
        jos[combined].push_back(Jos::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& mws = records["mwthinker Signal"];
        mws[construction].push_back(Mws::construction(N));
        mws[destruction].push_back(Mws::destruction(N));
        mws[connection].push_back(Mws::connection(N));
        mws[emission].push_back(Mws::emission(N));
        mws[combined].push_back(Mws::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nls = records["* neosigslot"];
        nls[construction].push_back(Nls::construction(N));
        nls[destruction].push_back(Nls::destruction(N));
        nls[connection].push_back(Nls::connection(N));
        nls[emission].push_back(Nls::emission(N));
        nls[combined].push_back(Nls::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nod = records["* fr00b0 nod"];
        nod[construction].push_back(Nod::construction(N));
        nod[destruction].push_back(Nod::destruction(N));
        nod[connection].push_back(Nod::connection(N));
        nod[emission].push_back(Nod::emission(N));
        nod[combined].push_back(Nod::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nss = records["Nano-signal-slot"];
        nss[construction].push_back(Nss::construction(N));
        nss[destruction].push_back(Nss::destruction(N));
        nss[connection].push_back(Nss::connection(N));
        nss[emission].push_back(Nss::emission(N));
        nss[combined].push_back(Nss::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& psg = records["pbhogan Signals"];
        psg[construction].push_back(Psg::construction(N));
        psg[destruction].push_back(Psg::destruction(N));
        psg[connection].push_back(Psg::connection(N));
        psg[emission].push_back(Psg::emission(N));
        psg[combined].push_back(Psg::combined(N));
        
        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& sss = records["supergrover sigslot"];
        sss[construction].push_back(Sss::construction(N));
        sss[destruction].push_back(Sss::destruction(N));
        sss[connection].push_back(Sss::connection(N));
        sss[emission].push_back(Sss::emission(N));
        sss[combined].push_back(Sss::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& wsg = records["* winglot Signals"];
        wsg[construction].push_back(Wsg::construction(N));
        wsg[destruction].push_back(Wsg::destruction(N));
        wsg[connection].push_back(Wsg::connection(N));
        wsg[emission].push_back(Wsg::emission(N));
        wsg[combined].push_back(Wsg::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& yas = records["Yassi"];
        yas[construction].push_back(Yas::construction(N));
        yas[destruction].push_back(Yas::destruction(N));
        yas[connection].push_back(Yas::connection(N));
        yas[emission].push_back(Yas::emission(N));
        yas[combined].push_back(Yas::combined(N));

        std::cout << "\n[Test Size: " << N << "] END" << std::endl;
    }

    //--------------------------------------------------------------------------

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

    // Output in unformatted csv

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

            ost << ", " << setprecision(0) << fixed << val;
        }
        ost << "\n";
    }
}
