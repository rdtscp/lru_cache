#include "../include/LruCache.h"

#include <benchmark/benchmark.h>

static void BM_Insert(benchmark::State &state) {
  const std::size_t capacity = state.range(0);
  const std::size_t numberItemsToInsert = capacity * 3;
  ads::LruCache<int, int> cache(capacity);
  for (auto _ : state) {
    for (std::size_t i = 0; i < numberItemsToInsert; ++i) {
      cache.insert(i, i);
    }
  }
  state.SetItemsProcessed(numberItemsToInsert);
}
BENCHMARK(BM_Insert)->Args({10000});

static void BM_Get(benchmark::State &state) {
  const std::size_t capacity = state.range(0);
  const std::size_t numberItemsToInsert = capacity * 3;
  ads::LruCache<int, int> cache(capacity);
  for (std::size_t i = 0; i < numberItemsToInsert; ++i) {
    cache.insert(i, i);
  }

  const std::size_t numberItemsToRead = capacity * 2;
  for (auto _ : state) {
    for (std::size_t i = 0; i < numberItemsToRead; ++i) {
      std::optional<int> result = cache.get(i);
      benchmark::DoNotOptimize(result);
    }
  }
  state.SetItemsProcessed(numberItemsToRead);
}
BENCHMARK(BM_Get)->Args({10000});

static void BM_TryGet(benchmark::State &state) {
  const std::size_t capacity = state.range(0);
  const std::size_t numberItemsToInsert = capacity * 3;
  ads::LruCache<int, int> cache(capacity);
  for (std::size_t i = 0; i < numberItemsToInsert; ++i) {
    cache.insert(i, i);
  }

  const std::size_t numberItemsToRead = capacity * 2;
  for (auto _ : state) {
    for (std::size_t i = 0; i < numberItemsToRead; ++i) {
      try {
        int result = cache.tryGet(i);
        benchmark::DoNotOptimize(result);
      } catch (...) {
      }
    }
  }
  state.SetItemsProcessed(numberItemsToRead);
}
BENCHMARK(BM_TryGet)->Args({10000});

BENCHMARK_MAIN();