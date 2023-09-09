#include "../include/Exceptions.h"
#include "../include/LruCache.h"

#include "gtest/gtest.h"

// The fixture for testing class Project1. From google test primer.
class Test_LruCache : public ::testing::Test {
protected:
  Test_LruCache() {
    // You can do set-up work for each test here.
  }
};

TEST_F(Test_LruCache, EmptyLruCacheCapacity) {
  const size_t capacity = 5;
  const ads::LruCache<int, int> cache(capacity);
  EXPECT_EQ(cache.capacity(), capacity);
}

TEST_F(Test_LruCache, EmptyLruCacheCopyCapacity) {
  const size_t capacity = 5;
  const ads::LruCache<int, int> cache(capacity);
  const ads::LruCache<int, int> cacheCopy = cache;
  EXPECT_EQ(cacheCopy.capacity(), capacity);
}

TEST_F(Test_LruCache, EmptyLruCacheMoveCapacity) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  const ads::LruCache<int, int> cacheMoved = std::move(cache);
  EXPECT_EQ(cacheMoved.capacity(), capacity);
}

TEST_F(Test_LruCache, EmptyLruCacheGetThrows) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  EXPECT_THROW(cache.tryGet(0), ads::KeyNotFoundException);
}

TEST_F(Test_LruCache, InsertAndGetSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  EXPECT_EQ(cache.tryGet(1), 100);
}

TEST_F(Test_LruCache, InsertAndOverwriteSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(1, 101);
  EXPECT_EQ(cache.tryGet(1), 101);
}

TEST_F(Test_LruCache, InsertMultipleAndSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(3), 300);
}

TEST_F(Test_LruCache, InsertBeyondCapacitySucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  cache.insert(5, 500);
  cache.insert(6, 600);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.tryGet(4), 400);
  EXPECT_EQ(cache.tryGet(5), 500);
  EXPECT_EQ(cache.tryGet(6), 600);
  EXPECT_THROW(cache.tryGet(1), ads::KeyNotFoundException);
}