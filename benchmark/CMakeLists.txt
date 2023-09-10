# Don't Install Google Test if its already installed.
IF(NOT GBENCHMARK_INSTALLED MATCHES TRUE)
  include(FetchContent)
  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
  )
  FetchContent_MakeAvailable(googlebenchmark)
ENDIF(NOT GBENCHMARK_INSTALLED MATCHES TRUE)

add_executable(runLruCacheBenchmarks Benchmark_LruCache.cpp)
target_link_libraries(runLruCacheBenchmarks benchmark::benchmark)