enable_testing()


add_executable(run_lru_cache_tests test_lru_cache.cpp)
target_link_libraries(run_lru_cache_tests GTest::gtest_main)
add_test(lru_cache_unit_tests run_lru_cache_tests)