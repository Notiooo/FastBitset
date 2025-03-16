#include "fastbitset/fastbitset.hpp"
#include <benchmark/benchmark.h>
#include <bitset>

static void BM_StdBitset(benchmark::State& state)
{
    for (auto _: state)
    {
        std::bitset<1000> bs;
        benchmark::DoNotOptimize(bs.size());
    }
}
BENCHMARK(BM_StdBitset);

static void BM_FastBitset(benchmark::State& state)
{
    for (auto _: state)
    {
        fastbitset::FastBitset<1000> fbs;
        benchmark::DoNotOptimize(fbs.size());
    }
}
BENCHMARK(BM_FastBitset);

BENCHMARK_MAIN();
