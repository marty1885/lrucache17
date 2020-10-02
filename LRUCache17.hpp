#pragma once
#include <algorithm>
#include <cstdint>
#include <list>
#include <shared_mutex>
#include <stdexcept>
#include <thread>
#include <optional>
#include <unordered_map>

namespace lru17 {
/*
 * a noop lockable concept that can be used in place of std::mutex
 */
class NullLock {
 public:
  void lock() {}
  void unlock() {}
  bool try_lock() { return true; }
  void lock_shared() {}
  void unlock_shared() {}
};

/**
 * error raised when a key not in cache is passed to get()
 */
class KeyNotFound : public std::invalid_argument {
 public:
  KeyNotFound() : std::invalid_argument("key_not_found") {}
};

template <typename K, typename V>
struct KeyValuePair {
 public:
  K key;
  V value;

  KeyValuePair(K k, V v) : key(std::move(k)), value(std::move(v)) {
  }
};

/**
 *	The LRU Cache class templated by
 *		Key - key type
 *		Value - value type
 *		MapType - an associative container like std::unordered_map
 *		LockType - a lock type derived from the Lock class (default:
 *NullLock = no synchronization)
 *
 *	The default NullLock based template is not thread-safe, however passing
 *Lock=std::mutex will make it
 *	thread-safe
 */
template <class Key, class Value, class Lock = NullLock,
          class Map = std::unordered_map<
              Key, typename std::list<KeyValuePair<Key, Value>>::iterator>>
class Cache {
 public:
  typedef KeyValuePair<Key, Value> node_type;
  typedef std::list<KeyValuePair<Key, Value>> list_type;
  typedef Map map_type;
  typedef Lock lock_type;
  using ReadGuard = std::shared_lock<lock_type>;
  using WriteGuard = std::unique_lock<lock_type>;
  /**
   * the maxSize is the soft limit of keys and (maxSize + elasticity) is the
   * hard limit
   * the cache is allowed to grow till (maxSize + elasticity) and is pruned back
   * to maxSize keys
   * set maxSize = 0 for an unbounded cache (but in that case, you're better off
   * using a std::unordered_map
   * directly anyway! :)
   */
  explicit Cache(size_t maxSize = 64, size_t elasticity = 10)
      : maxSize_(maxSize), elasticity_(elasticity) {}
  virtual ~Cache() = default;
  size_t size() const {
    ReadGuard g(lock_);
    return cache_.size();
  }
  bool empty() const {
    ReadGuard g(lock_);
    return cache_.empty();
  }
  void clear() {
    WriteGuard l(lock_);
    cache_.clear();
    keys_.clear();
  }
  void insert(const Key& k, Value v) {
    WriteGuard l(lock_);
    const auto iter = cache_.find(k);
    if (iter != cache_.end()) {
      iter->second->value = v;
      keys_.splice(keys_.begin(), keys_, iter->second);
      return;
    }

    keys_.emplace_front(k, std::move(v));
    cache_[k] = keys_.begin();
    prune();
  }
  std::optional<Value> tryGet(const Key& kIn) {
    ReadGuard g(lock_);
    const auto iter = cache_.find(kIn);
    if (iter == cache_.end()) {
      return {};
    }
    keys_.splice(keys_.begin(), keys_, iter->second);
    return iter->second->value;
  }
  /**
   *	The const reference returned here is only
   *    guaranteed to be valid till the next insert/delete
   */
  const Value& get(const Key& k) {
    ReadGuard g(lock_);
    const auto iter = cache_.find(k);
    if (iter == cache_.end()) {
      throw KeyNotFound();
    }
    keys_.splice(keys_.begin(), keys_, iter->second);
    return iter->second->value;
  }
  /**
   * returns a copy of the stored object (if found)
   */
  Value getCopy(const Key& k) {
   return get(k);
  }
  bool remove(const Key& k) {
    WriteGuard g(lock_);
    auto iter = cache_.find(k);
    if (iter == cache_.end()) {
      return false;
    }
    // keys_.erase(iter->second);
    cache_.erase(iter);
    return true;
  }
  bool contains(const Key& k) const {
    ReadGuard g(lock_);
    return cache_.find(k) != cache_.end();
  }

  size_t getMaxSize() const { return maxSize_; }
  size_t getElasticity() const { return elasticity_; }
  size_t getMaxAllowedSize() const { return maxSize_ + elasticity_; }
  template <typename F>
  void cwalk(F& f) const {
    ReadGuard g(lock_);
    std::for_each(keys_.begin(), keys_.end(), f);
  }

 protected:
  size_t prune() {
    size_t maxAllowed = maxSize_ + elasticity_;
    if (maxSize_ == 0 || cache_.size() < maxAllowed) {
      return 0;
    }
    size_t count = 0;
    while (cache_.size() > maxSize_) {
      cache_.erase(keys_.back().key);
      keys_.pop_back();
      ++count;
    }
    return count;
  }

 private:
  // Disallow copying.
  Cache(const Cache&) = delete;
  Cache& operator=(const Cache&) = delete;

  mutable Lock lock_;
  Map cache_;
  list_type keys_;
  size_t maxSize_;
  size_t elasticity_;
};

}  // namespace LRUCache11
