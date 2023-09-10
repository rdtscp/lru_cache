# Don't Install Google Test if its already installed.
IF(NOT GBENCHMARK_INSTALLED MATCHES TRUE)
  include(FetchContent)
  FetchContent_Declare(
    googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.zip
  )
  FetchContent_MakeAvailable(googlebenchmark)
ENDIF(NOT GBENCHMARK_INSTALLED MATCHES TRUE)

add_executable(run_lru_cache_benchmark benchmark_lru_cache.cpp)
target_link_libraries(run_lru_cache_benchmark benchmark::benchmark)