#include "TimedLRUCache17.hpp"

#include <iostream>
#include <thread>
#include <optional>
#include <cassert>

int main()
{
	using namespace std::chrono_literals;
	lru17::TimedCache<std::string, std::string> cache(3s, 5, 0);
	cache.insert("Hello", "World");
	std::cout << cache.unsafeGet("Hello") << std::endl;
	std::this_thread::sleep_for(4s);
	std::optional<std::string> res = cache.tryGet("Hello");
	assert(res.has_value() == false);
	std::cout << "Value is no longer avaliable after 3 seconds!!" << std::endl;


}