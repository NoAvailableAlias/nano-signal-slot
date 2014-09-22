// MSVC no, I do not want to use localtime_s
#define _CRT_SECURE_NO_WARNINGS

#include "benchmark.hpp"

#include "asg_benchmark.hpp"
#include "bs1_benchmark.hpp"
#include "bs2_benchmark.hpp"
#include "evf_benchmark.hpp"
#include "evl_benchmark.hpp"
#include "evs_benchmark.hpp"
#include "jls_benchmark.hpp"
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
//#include <sstream>
//#include <vector>
#include <map>

const char* construction = "construct";
const char* destruction = "destruct";
const char* connection = "connect";
const char* emission = "emission";
const char* combined = "combined";

using ImmediateResults = std::map<const char*, std::vector<double>>;
using RelativeResults = std::map<const char*, double>;
using ImmediateData = std::map<const char*, ImmediateResults>;
using RelativeData = std::map<const char*, RelativeResults>;

//std::size_t g_limit = Timer_u(Limit_u(4)).count();
std::size_t g_limit = 10000000;

void outputReports(ImmediateData const&);
template <typename T> void outputReportTxt(ImmediateData const&, T&);
template <typename T> void outputReportCsv(ImmediateData const&, T&);

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

    std::cout << "Set CPU Priority: [paused]" << std::endl;
    std::cin.get();

    for(std::size_t N = start_n; N <= maximum_n; N *= 2)
    {
        std::cout << "[Test Size: " << N << "] BEGIN\n" << std::endl;

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& wsg = records["winglot Signals"];
        wsg[construction].emplace_back(Wsg::construction(N));
        wsg[destruction].emplace_back(Wsg::destruction(N));
        wsg[connection].emplace_back(Wsg::connection(N));
        wsg[emission].emplace_back(Wsg::emission(N));
        wsg[combined].emplace_back(Wsg::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& sss = records["supergrover sigslot"];
        sss[construction].emplace_back(Sss::construction(N));
        sss[destruction].emplace_back(Sss::destruction(N));
        sss[connection].emplace_back(Sss::connection(N));
        sss[emission].emplace_back(Sss::emission(N));
        sss[combined].emplace_back(Sss::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& psg = records["pbhogan Signals"];
        psg[construction].emplace_back(Psg::construction(N));
        psg[destruction].emplace_back(Psg::destruction(N));
        psg[connection].emplace_back(Psg::connection(N));
        psg[emission].emplace_back(Psg::emission(N));
        psg[combined].emplace_back(Psg::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& jls = records["Jl_signal"];
        jls[construction].emplace_back(Jls::construction(N));
        jls[destruction].emplace_back(Jls::destruction(N));
        jls[connection].emplace_back(Jls::connection(N));
        jls[emission].emplace_back(Jls::emission(N));
        jls[combined].emplace_back(Jls::combined(N));

        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& nss = records["Nano-signal-slot"];
        nss[construction].emplace_back(Nss::construction(N));
        nss[destruction].emplace_back(Nss::destruction(N));
        nss[connection].emplace_back(Nss::connection(N));
        nss[emission].emplace_back(Nss::emission(N));
        nss[combined].emplace_back(Nss::combined(N));

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

        auto& evs = records["EvilTwin Obs Safe"];
        evs[construction].emplace_back(Evs::construction(N));
        evs[destruction].emplace_back(Evs::destruction(N));
        evs[connection].emplace_back(Evs::connection(N));
        evs[emission].emplace_back(Evs::emission(N));
        evs[combined].emplace_back(Evs::combined(N));
        
        std::cout << "[Line: " << __LINE__ << "]" << std::endl;

        auto& s11 = records["amc522 Signal11"];
        s11[construction].emplace_back(Asg::construction(N));
        s11[destruction].emplace_back(Asg::destruction(N));
        s11[connection].emplace_back(Asg::connection(N));
        s11[emission].emplace_back(Asg::emission(N));
        s11[combined].emplace_back(Asg::combined(N));

        std::cout << "\n[Test Size: " << N << "] END" << std::endl;
    }
    outputReports(records);

    std::size_t numImplementations = records.size();
    std::size_t numOperations = records.begin()->second.size();
    std::size_t numRounds = records.begin()->second.begin()->second.size();
    std::size_t numBenchmarks = numImplementations * numOperations * numRounds;

    std::cout << "\n" << numBenchmarks << " Benchmarks Completed: [paused]" << std::endl;
    std::cin.get();
}

//------------------------------------------------------------------------------

void outputReports(ImmediateData const& records)
{
    if (std::ofstream ofs { "report.txt", std::ios::app })
    {
        auto tee = std::tie(std::cout, ofs);

        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        tee << std::put_time(std::localtime(&now_c), "%c") << '\n';

        outputReportTxt(records, tee);

        ofs << "\n//--------------------------------------"
            << "----------------------------------------\n" << std::endl;
    }
    else
    {
        std::cerr << "Unable to append report.txt: [error]" << std::endl;
    }
    if (std::ofstream ofs { "report.csv" })
    {
        outputReportCsv(records, ofs);
    }
    else
    {
        std::cerr << "Unable to create report.csv: [error]" << std::endl;
    }
}

//------------------------------------------------------------------------------

template <typename T>
void outputReportTxt(ImmediateData const& records, T& ost)
{
    using namespace std;

    for(auto const& table : records)
    {
        auto const& tableName = table.first;

        ost << "\n    " << tableName << ":";

        for(auto const& record : table.second)
        {
            auto const& recordName = record.first;
            auto const& data = record.second;

            double avg = accumulate(begin(data),
                end(data), 1.0) / (double) data.size();

            ost << "\n ++ " << setw(40) << setfill('_')
                << recordName << ": " << setprecision(2) << fixed << avg;
        }
        ost << "\n";
    }
}

//------------------------------------------------------------------------------

template <typename T>
void outputReportCsv(ImmediateData const& records, T& ost)
{
    using namespace std;

    ost << "Library," << construction << "," << destruction << ","
        << connection << "," << emission << "," << combined << endl;

    for(auto const& table : records)
    {
        ost << table.first;

        for(auto const& record : table.second)
        {
            auto const& data = record.second;

            double avg = accumulate(begin(data),
                end(data), 1.0) / (double) data.size();

            ost << "," << setprecision(2) << fixed << avg;
        }
        ost << endl;
    }
}

//-------------------------------------------------------------CAUTION YOUR EYES

/*
template <typename T>
void outputReportNew(ImmediateData const& records, T& ost)
{
    RelativeResults maxAverage;
    RelativeData averages;

    const std::size_t maxOutputLength = 80;
    std::size_t maxNameLength = 0;

    std::map<std::string, std::size_t> maxColumnLength;

    for(auto const& table : records)
    {
        auto const& tableName = table.first;

        maxNameLength = std::max(maxNameLength, tableName.size() + 2);

        for(auto const& record : table.second)
        {
            auto const& recordName = record.first;
            auto const& data = record.second;

            double avg = std::accumulate(std::begin(data),
                std::end(data), 1.0) / (double) data.size();

            averages[tableName][recordName] = avg;

            maxAverage[recordName] = std::max(maxAverage[recordName], avg);

            std::stringstream sts;
            sts << std::setprecision(2) << std::fixed << avg;
            maxColumnLength[recordName] = std::max(maxColumnLength[recordName], sts.str().size() + 2);
        }
    }

    for(auto const& table : averages)
    {
        auto const& tableName = table.first;

        ost << "+" << std::string(maxNameLength, '-') << "+\n|";

        std::size_t currentNameLength = tableName.size();
        std::size_t namePaddingLength = maxNameLength / currentNameLength;
        bool extraSpace = (namePaddingLength * currentNameLength == maxNameLength);
        std::string namePadding(namePaddingLength, ' ');

        ost << namePadding << tableName << namePadding + (extraSpace ? " |" : "|");

        std::vector<double> dt;

        for(auto const& record : table.second)
        {
            auto const& recordName = record.first;
            auto const& average = record.second;

            dt.emplace_back((average / maxAverage[recordName]) * 100.0);

            std::size_t columnLength = maxColumnLength[recordName];
            std::size_t valuePaddingLength = columnLength / recordName.size();


        }
    }
}
*/
