enable_testing()


add_executable(runLruCacheTests Test_LruCache.cpp)
target_link_libraries(runLruCacheTests GTest::gtest_main)
add_test(LruCacheUnitTests runLruCacheTests)

add_executable(runExceptionsTests Test_Exceptions.cpp)
target_link_libraries(runExceptionsTests GTest::gtest_main)
add_test(ExceptionsUnitTests runExceptionsTests)