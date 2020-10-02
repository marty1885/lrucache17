# LRUCache17

A header only C++17 LRU Cache template class that allows you to define key, value and optionally the Map type. uses a double linked list and a `std::unordered_map` style container to provide fast insert, delete and update. It also comes with a timed cache that invalidates entries after a set amount of time. Timed cache isn't wiritten in the optimal way but should be more than enought to handle day to day works.

No dependencies other than the C++ standard library. This is a C++17 fork of the old LRUCache11 library.

The goal was to create a fast LRUCache header only library and to avoid any dependencies like boost.

Enjoy and drop me a line.


## Usage Example

```cpp
#include "LRUCache17.hpp"
namespace lru
{
	void test()
	{
		lru17::Cache<std::string, std::string> cache(3,0);
		cache.insert("hello", "world");
		cache.insert("foo", "bar");
		
		std::cout<<"checking refresh : "<<cache.get("hello")<<std::endl;
		cache.insert("hello1", "world1");
		cache.insert("foo1", "bar1");
	}
}

int main(int argc, char **argv)
{
	lru::test();
	return 0;
}
```

Build with `g++ -o SampleMain -std=c++17 SampleMain.cpp -std=c++17`

## License

BSD License

```
/*
 * LRUCache17 - a templated C++17 based LRU cache class that allows specification of
 * key, value and optionally the map container type (defaults to std::unordered_map)
 * By using the std::map and a linked list of keys it allows O(1) insert, delete and
 * refresh operations.
 *
 * This is a header-only library and all you need is the LRUCache11.hpp file
 *
 * Github: https://github.com/marty1885/lrucache17
 *
 *
 * Copyright (c) 2012-22 SAURAV MOHAPATRA <mohaps@gmail.com>
 * Copyright (c) 2020 MARTIN CHANG <marty188586@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
```

## Links

* Wikipedia Entry on LRU Caching : http://en.wikipedia.org/wiki/Cache_algorithms#Least_Recently_Used
* LRUCache11 - the library that LRUCache17 based on : https://github.com/mohaps/lrucache11
