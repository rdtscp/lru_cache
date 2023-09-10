enable_testing()


add_executable(runLruCacheTests Test_LruCache.cpp)
target_link_libraries(runLruCacheTests GTest::gtest_main)
add_test(LruCacheUnitTests runLruCacheTests)