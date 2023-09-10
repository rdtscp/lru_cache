
#include "../include/LruCache.h"

#include "gtest/gtest.h"

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
  EXPECT_EQ(cache.find(0), cache.end());
}

TEST_F(Test_LruCache, InsertAndGetSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.size(), 1u);
}

TEST_F(Test_LruCache, InsertAndOverwriteSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(1, 101);
  EXPECT_EQ(cache.find(1)->second, 101);
  EXPECT_EQ(cache.size(), 1u);
}

TEST_F(Test_LruCache, InsertMultipleAndSucceeds) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(2)->second, 200);
  EXPECT_EQ(cache.find(3)->second, 300);
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
  EXPECT_EQ(cache.find(2)->second, 200);
  EXPECT_EQ(cache.find(3)->second, 300);
  EXPECT_EQ(cache.find(4)->second, 400);
  EXPECT_EQ(cache.find(5)->second, 500);
  EXPECT_EQ(cache.find(6)->second, 600);
  EXPECT_EQ(cache.find(1), cache.end());
}

TEST_F(Test_LruCache, GetUpdatesMru) {
  const size_t capacity = 3;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(2)->second, 200);
  EXPECT_EQ(cache.find(3)->second, 300);
  EXPECT_EQ(cache.size(), 3u);

  // Update the MRU
  cache.find(1);
  cache.find(3);
  cache.insert(4, 400);
  EXPECT_EQ(cache.size(), 3u);
  EXPECT_EQ(cache.find(2), cache.end());
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(3)->second, 300);
  EXPECT_EQ(cache.find(4)->second, 400);
}

TEST_F(Test_LruCache, Evict) {
  const size_t capacity = 3;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(2)->second, 200);
  EXPECT_EQ(cache.find(3)->second, 300);
  EXPECT_EQ(cache.size(), 3u);

  // Update the MRU
  cache.erase(2);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(2), cache.end());
  EXPECT_EQ(cache.find(3)->second, 300);
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
  cache.erase(3);
  EXPECT_EQ(cache.size(), 4u);
  cache.insert(6, 600);
  EXPECT_EQ(cache.size(), 5u);
  EXPECT_EQ(cache.find(1)->second, 100);
  EXPECT_EQ(cache.find(2)->second, 200);
  EXPECT_EQ(cache.find(4)->second, 400);
  EXPECT_EQ(cache.find(5)->second, 500);
  EXPECT_EQ(cache.find(6)->second, 600);
  EXPECT_EQ(cache.find(3), cache.end());
}

TEST_F(Test_LruCache, ClearAndEmpty) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  cache.insert(5, 500);
  EXPECT_FALSE(cache.empty());
  EXPECT_EQ(cache.size(), 5u);
  cache.erase(3);
  EXPECT_EQ(cache.size(), 4u);
  cache.insert(6, 600);
  EXPECT_EQ(cache.size(), 5u);
  cache.clear();
  EXPECT_EQ(cache.size(), 0u);
  EXPECT_TRUE(cache.empty());
}

TEST_F(Test_LruCache, Iterators) {
  const size_t capacity = 5;
  ads::LruCache<int, int> cache(capacity);
  cache.insert(1, 100);
  cache.insert(2, 200);
  cache.insert(3, 300);
  cache.insert(4, 400);
  cache.insert(5, 500);
  const std::vector<std::pair<int, int>> expectedKeyValues = {
      std::make_pair(5, 500), std::make_pair(4, 400), std::make_pair(3, 300),
      std::make_pair(2, 200), std::make_pair(1, 100)};
  std::vector<std::pair<int, int>> actualKeyValues;
  for (const auto &item : cache) {
    actualKeyValues.push_back(item);
  }
  EXPECT_EQ(expectedKeyValues, actualKeyValues);
}

TEST_F(Test_LruCache, ConstIterators) {
  const size_t capacity = 5;
  ads::LruCache<int, int> tempCache(capacity);
  tempCache.insert(1, 100);
  tempCache.insert(2, 200);
  tempCache.insert(3, 300);
  tempCache.insert(4, 400);
  tempCache.insert(5, 500);
  const ads::LruCache<int, int> cache = std::move(tempCache);
  const std::vector<std::pair<int, int>> expectedKeyValues = {
      std::make_pair(5, 500), std::make_pair(4, 400), std::make_pair(3, 300),
      std::make_pair(2, 200), std::make_pair(1, 100)};
  std::vector<std::pair<int, int>> actualKeyValues;
  for (const auto &item : cache) {
    actualKeyValues.push_back(item);
  }
  EXPECT_EQ(expectedKeyValues, actualKeyValues);
}