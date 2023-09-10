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
  EXPECT_EQ(cache.size(), 0u);
}

TEST_F(Test_LruCache, EmptyLruCacheCopyCapacity) {
  const size_t capacity = 5;
  const ads::LruCache<int, int> cache(capacity);
  const ads::LruCache<int, int> cacheCopy = cache;
  EXPECT_EQ(cacheCopy.capacity(), capacity);
  EXPECT_EQ(cacheCopy.size(), 0u);
}

TEST_F(Test_LruCache, EmptyLruCacheMoveCapacity) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  const ads::LruCache<int, int> cacheMoved = std::move(cache);
  EXPECT_EQ(cacheMoved.capacity(), capacity);
  EXPECT_EQ(cacheMoved.size(), 0u);
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
  EXPECT_EQ(cache.size(), 1u);
}

TEST_F(Test_LruCache, InsertAndOverwriteSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(1, 101);
  EXPECT_EQ(cache.tryGet(1), 101);
  EXPECT_EQ(cache.size(), 1u);
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
  EXPECT_EQ(cache.size(), 3u);
}

TEST_F(Test_LruCache, InsertBeyondCapacitySucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  EXPECT_EQ(cache.size(), 4u);
  cache.insert(5, 500);
  EXPECT_EQ(cache.size(), 5u);
  cache.insert(6, 600);
  EXPECT_EQ(cache.size(), 5u);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.tryGet(4), 400);
  EXPECT_EQ(cache.tryGet(5), 500);
  EXPECT_EQ(cache.tryGet(6), 600);
  EXPECT_THROW(cache.tryGet(1), ads::KeyNotFoundException);
}

TEST_F(Test_LruCache, GetUpdatesMru) {
  const size_t capacity = 3;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.size(), 3u);

  // Update the MRU
  cache.tryGet(1);
  cache.tryGet(3);
  cache.insert(4, 400);
  EXPECT_EQ(cache.size(), 3u);
  EXPECT_THROW(cache.tryGet(2), ads::KeyNotFoundException);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.tryGet(4), 400);
}

TEST_F(Test_LruCache, Evict) {
  const size_t capacity = 3;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.size(), 3u);

  // Update the MRU
  cache.evict(2);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_THROW(cache.tryGet(2), ads::KeyNotFoundException);
  EXPECT_EQ(cache.tryGet(3), 300);
  EXPECT_EQ(cache.size(), 2u);
}

TEST_F(Test_LruCache, InsertAfterEvictCapacitySucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  cache.insert(5, 500);
  EXPECT_EQ(cache.size(), 5u);
  cache.evict(3);
  EXPECT_EQ(cache.size(), 4u);
  cache.insert(6, 600);
  EXPECT_EQ(cache.size(), 5u);
  EXPECT_EQ(cache.tryGet(1), 100);
  EXPECT_EQ(cache.tryGet(2), 200);
  EXPECT_EQ(cache.tryGet(4), 400);
  EXPECT_EQ(cache.tryGet(5), 500);
  EXPECT_EQ(cache.tryGet(6), 600);
  EXPECT_THROW(cache.tryGet(3), ads::KeyNotFoundException);
}

TEST_F(Test_LruCache, GetOptionalApi) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  cache.insert(5, 500);
  EXPECT_EQ(cache.size(), 5u);
  cache.evict(3);
  EXPECT_EQ(cache.size(), 4u);
  cache.insert(6, 600);
  EXPECT_EQ(cache.size(), 5u);
  EXPECT_EQ(cache.get(1).value(), 100);
  EXPECT_EQ(cache.get(2).value(), 200);
  EXPECT_EQ(cache.get(4).value(), 400);
  EXPECT_EQ(cache.get(5).value(), 500);
  EXPECT_EQ(cache.get(6).value(), 600);
  EXPECT_EQ(cache.get(3), std::nullopt);
}
