#pragma once

#include <cstddef>
#include <list>
#include <stdexcept>
#include <unordered_map>

#include "Exceptions.h"

namespace ads {

template <typename KeyT, typename ValueT> class LruCache {
private:
  /* Type definitions for internals. */
  template <typename T, typename V> using MapT = std::unordered_map<T, V>;
  using ValueContainer = std::list<ValueT>;
  using ValueContainerIterator = typename ValueContainer::iterator;
  using KeyToValueContainerIteratorIndex = MapT<KeyT, ValueContainerIterator>;

public:
  LruCache(const std::size_t capacity) : capacity_(capacity) {}

public:
  void insert(const KeyT &key, const ValueT &value) {
    // Two scenarios:
    //   1. We have never seen this key before.
    //      - Insert as MRU
    //      - Update index
    //      - If over-capacity, evict LRU
    //   2. We have seen this key before.
    //      - Replace its value
    //      - Mark as MRU
    //      - If over-capacity, evict LRU
    const auto it = keyToValueIndex_.find(key);
    // We haven't seen this key before.
    if (it == keyToValueIndex_.end()) {
      orderedValues_.emplace_front(value);
      keyToValueIndex_[key] = orderedValues_.begin();
    } else {
      ValueContainerIterator valueIterator = it->second;
      *valueIterator = value;
      keyToValueIndex_.try_emplace(key, valueIterator);
    }
  }

  /* public mutable API */
  const ValueT &tryGet(const KeyT &key) {
    const auto it = keyToValueIndex_.find(key);
    if (it == keyToValueIndex_.end()) {
      throw ads::KeyNotFoundException(std::to_string(key));
    }

    const ValueContainerIterator &valueIterator = it->second;
    return *valueIterator;
  }

public:
  /* public const API */
  std::size_t capacity() const { return capacity_; }

private:
  const std::size_t capacity_;
  ValueContainer orderedValues_;
  KeyToValueContainerIteratorIndex keyToValueIndex_;
};

} // namespace ads
