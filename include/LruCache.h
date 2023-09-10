#pragma once

#include <cstddef>
#include <iterator>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>

#include "Exceptions.h"

namespace ads {

template <typename KeyT, typename ValueT> class LruCache {
private:
  /* Type definitions for internals. */
  template <typename T, typename V> using MapT = std::unordered_map<T, V>;
  using ValueContainer = std::list<std::pair<KeyT, ValueT>>;
  using ValueContainerIterator = typename ValueContainer::iterator;
  using KeyToValueContainerIteratorIndex = MapT<KeyT, ValueContainerIterator>;

public:
  LruCache(const std::size_t capacity) : capacity_(capacity) {}

public:
  /* public mutable API */

  void insert(const KeyT &key, const ValueT &value) {
    // Two scenarios:
    //   1. We have never seen this key before.
    //      - Insert as MRU
    //      - Update index
    //      - If over-capacity, evict LRU
    //   2. We have seen this key before.
    //      - Replace its value
    //      - Mark as MRU
    const auto it = keyToValueIndex_.find(key);
    // We haven't seen this key before.
    if (it == keyToValueIndex_.end()) {
      orderedValues_.emplace_front(key, value);
      keyToValueIndex_[key] = orderedValues_.begin();
      maybeEvictLru();
    } else {
      ValueContainerIterator valueIterator = it->second;
      valueIterator->second = value;
      markMRU(valueIterator);
    }
  }

  void evict(const KeyT &key) {
    const auto it = keyToValueIndex_.find(key);
    if (it == keyToValueIndex_.end()) {
      return;
    }

    orderedValues_.erase(it->second);
    keyToValueIndex_.erase(it);
  }

  const ValueT &tryGet(const KeyT &key) {
    const auto it = keyToValueIndex_.find(key);
    if (it == keyToValueIndex_.end()) {
      throw ads::KeyNotFoundException(std::to_string(key));
    }

    const ValueContainerIterator &valueIterator = it->second;
    markMRU(valueIterator);
    return valueIterator->second;
  }

  const std::optional<ValueT> get(const KeyT &key) {
    const auto it = keyToValueIndex_.find(key);
    if (it == keyToValueIndex_.end()) {
      return std::nullopt;
    }

    const ValueContainerIterator &valueIterator = it->second;
    markMRU(valueIterator);
    return valueIterator->second;
  }

  void clear() {
    orderedValues_.clear();
    keyToValueIndex_.clear();
  }

  typename ValueContainer::iterator begin() { return orderedValues_.begin(); }

  typename ValueContainer::iterator end() { return orderedValues_.end(); }

public:
  /* public const API */
  std::size_t capacity() const { return capacity_; }

  std::size_t size() const { return keyToValueIndex_.size(); }

  bool empty() const { return size() == 0u; }

  typename ValueContainer::const_iterator begin() const {
    return orderedValues_.begin();
  }

  typename ValueContainer::const_iterator end() const {
    return orderedValues_.end();
  }

  typename ValueContainer::const_iterator cbegin() const {
    return orderedValues_.cbegin();
  }

  typename ValueContainer::const_iterator cend() const {
    return orderedValues_.cend();
  }

private:
  void maybeEvictLru() {
    if (keyToValueIndex_.size() <= capacity_) {
      return;
    }

    const auto lruItemIt = orderedValues_.rbegin();
    keyToValueIndex_.erase(lruItemIt->first);
    orderedValues_.resize(capacity_);
  }

  void markMRU(ValueContainerIterator valueIterator) {
    if (valueIterator != orderedValues_.begin()) {
      orderedValues_.splice(orderedValues_.begin(), orderedValues_,
                            valueIterator, std::next(valueIterator));
    }
  }

private:
  const std::size_t capacity_;
  ValueContainer orderedValues_;
  KeyToValueContainerIteratorIndex keyToValueIndex_;
};

} // namespace ads
