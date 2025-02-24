#include "fastbitset/fastbitset.hpp"
#include <benchmark/benchmark.h>
#include <bitset>

static void BM_StdBitset(benchmark::State& state)
{
    for (auto _: state)
    {
        std::bitset<1000> bs;
        bs.size();
        benchmark::DoNotOptimize(bs);
    }
}
BENCHMARK(BM_StdBitset);

static void BM_FastBitset(benchmark::State& state)
{
    for (auto _: state)
    {
        fastbitset::FastBitset fbs;
        fbs.size();
        benchmark::DoNotOptimize(fbs);
    }
}
BENCHMARK(BM_FastBitset);

BENCHMARK_MAIN();
