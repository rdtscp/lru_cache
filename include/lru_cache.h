#pragma once

#include <cstddef>
#include <iterator>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>

namespace ads {

template <typename K, typename V> class lru_cache {
private:
  /* Type definitions for internals. */
  using value_container = std::list<std::pair<K, V>>;
  using key_index = std::unordered_map<K, typename value_container::iterator>;

public:
  lru_cache(const std::size_t capacity) : capacity_(capacity) {}

  typename value_container::const_iterator find(const K &key) const {
    const auto it = key_to_value_index_.find(key);
    if (it == key_to_value_index_.end()) {
      return end();
    }

    const typename value_container::const_iterator &value_it = it->second;
    mark_mru(value_it);

    return value_it;
  }

  std::size_t capacity() const { return capacity_; }

  std::size_t size() const { return key_to_value_index_.size(); }

  bool empty() const { return size() == 0u; }

  void insert(const K &key, const V &value) {
    // Two scenarios:
    //   1. We have never seen this key before.
    //      - Insert as MRU
    //      - Update index
    //      - If over-capacity, evict LRU
    //   2. We have seen this key before.
    //      - Replace its value
    //      - Mark as MRU
    const auto it = key_to_value_index_.find(key);
    if (it == key_to_value_index_.end()) {
      ordered_values_.emplace_front(key, value);
      key_to_value_index_[key] = ordered_values_.begin();
      maybe_evict_lru();
    } else {
      typename value_container::iterator value_iterator = it->second;
      value_iterator->second = value;
      mark_mru(value_iterator);
    }
  }

  void erase(const K &key) {
    const auto it = key_to_value_index_.find(key);
    if (it == key_to_value_index_.end()) {
      return;
    }

    ordered_values_.erase(it->second);
    key_to_value_index_.erase(it);
  }

  void erase(const typename value_container::iterator &it) { erase(it->first); }
  void erase(const typename value_container::const_iterator &it) {
    erase(it->first);
  }

  void clear() {
    ordered_values_.clear();
    key_to_value_index_.clear();
  }

  typename value_container::iterator begin() { return ordered_values_.begin(); }

  typename value_container::iterator end() { return ordered_values_.end(); }

  typename value_container::const_iterator begin() const {
    return ordered_values_.begin();
  }

  typename value_container::const_iterator end() const {
    return ordered_values_.end();
  }

  typename value_container::const_iterator cbegin() const {
    return ordered_values_.cbegin();
  }

  typename value_container::const_iterator cend() const {
    return ordered_values_.cend();
  }

private:
  void maybe_evict_lru() {
    if (key_to_value_index_.size() <= capacity_) {
      return;
    }

    const auto lru_item_it = ordered_values_.rbegin();
    key_to_value_index_.erase(lru_item_it->first);
    ordered_values_.resize(capacity_);
  }

  void mark_mru(typename value_container::const_iterator it) const {
    if (it != ordered_values_.begin()) {
      ordered_values_.splice(ordered_values_.begin(), ordered_values_, it,
                             std::next(it));
    }
  }

private:
  const std::size_t capacity_;
  mutable value_container ordered_values_;
  key_index key_to_value_index_;
};

} // namespace ads
