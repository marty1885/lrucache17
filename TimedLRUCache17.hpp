#pragma once
#include "LRUCache17.hpp"

#include <chrono>

namespace lru17
{

template <class Key, class Value, class Lock = NullLock,
          class Duration = std::chrono::duration<double, std::milli>>
class TimedCache
{
public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using clock_type = Clock;
  using time_type = TimePoint;
  using key_type = Key;
  using value_type = Value;
  using lock_type = Lock;

  explicit TimedCache(Duration timeToLive, size_t maxSize = 64, size_t elasticity = 10)
    : cache_(maxSize, elasticity), timeToLive_(timeToLive) {}
  void clear() {
    cache_.clear();
  }
  bool contains(const Key& k) {
    return timeToLive(k);
  }
  template <typename F>
  void cwalk(F& f) const {
    cache_.cwalk(f);
  }
  bool empty() const {
    return cache_.empty();
  }
  const Value& unsafeGet(const Key& k) {
    const auto& [value, time] = cache_.get(k);
    if(Clock::now()-time > timeToLive_) {
      remove(k);
      throw KeyNotFound();
    }
    return value;
  }
  Value getCopy(const Key& k) {
    return get(k);
  }
  size_t getMaxSize() const { return cache_.getMaxSize(); }
  size_t getElasticity() const { return cache_.getElasticity(); }
  size_t getMaxAllowedSize() const { return cache_.getMaxAllowedSize(); }
  void insert(const Key& k, Value v) {
    cache_.insert(k, {v, Clock::now()});
  }
  std::optional<Value> tryGet(const Key& k) {
    auto value_time_pair = cache_.tryGet(k);
    if(value_time_pair.has_value() == false)
      return {};
    else if(Clock::now() - value_time_pair.value().second > timeToLive_) {
      remove(k);
      return {};
    }
    return value_time_pair.value().first;
  }
  bool remove(const Key& k) {
    return cache_.remove(k);
  }
  size_t size() const {
    return cache_.size();
  }
private:
  Cache<Key, std::pair<Value, TimePoint>, Lock> cache_;
  Duration timeToLive_;
};

}