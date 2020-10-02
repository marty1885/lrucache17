// tests LRUCache with vectors as key
// NOTE: DOESN'T WORK. DON'T CARE
#include<vector>
#include<map>
#include <shared_mutex>
#include "LRUCache17.hpp"

#include<cassert>

using namespace std;

// types for our cache
using KeyT = vector<int>;
using ValueT = vector<unsigned int>;
using KVMap = map<KeyT, ValueT>;

int main() {
  lru17::Cache<KeyT, ValueT, std::shared_mutex, KVMap> cache;

  const KeyT key1{1,2,3}; //must be const for LRUCache(?)
  const ValueT val1{0,0,1};
  cache.insert(key1, val1);

  auto ret = cache.get(key1);
  assert (ret == val1);

  return 0;
}
