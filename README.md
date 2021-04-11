MimicMap
========

The sorted associative container based on std::vector that mimicking std::map.

Usage
-----

Almost same as std::map,
but all container elements are stored in single std::vector.
If you insert sporadic key element,
MimicMap automatically complete gap elements in std::vector.

You can avoid out of memory by setting lower/higher limit of keys
(setLowerLimit/setHigherLimit member functions).

Member types
------------

|Member type              |Definition                                        |
------------------------- | ------------------------------------------------
|`value_type`             |`K`                                               |
|`mapped_type`            |`V`                                               |
|`value_type`             |`std::pair<K, V>`                                 |
|`iterator`               |`std::vector<value_type>::iterator`               |
|`const_iterator`         |`std::vector<value_type>::const_iterator`         |
|`reverse_iterator`       |`std::vector<value_type>::reverse_iterator`       |
|`const_reverse_iterator` |`std::vector<value_type>::const_reverse_iterator` |
|`allocator_type`         |`std::vector<value_type>::allocator_type`         |

Member functions
----------------

Member functions almost same as std::map:

|Member function                                                |Description |
| ------------------------------------------------------------- | ---------- |
|`MimicMap(const allocator_type& allocator = allocator_type())` |constructor |
|`MimicMap(const MimicMap<K, V>& orig)`                         |constructor |
|`template<typename IT> MimicMap(IT first, IT last, const allocator_type& allocator = allocator_type())` |constructor |
|`MimicMap<K, V>& operator=(const MimicMap<K, V>& orig)` |copy assign operator |
|`void swap(MimicMap<K, V>& other)` |swaps the contents |
|`size_t size() const`              |returns the number of elements |
|`void clear()`                     |clears the contents |
|`iterator begin()`                 |returns an iterator to the beginning |
|`const_iterator begin() const`     |returns an iterator to the beginning |
|`const_iterator cbegin() const`    |returns an iterator to the beginning |
|`iterator end()`                   |returns an iterator to the end |
|`const_iterator end() const`       |returns an iterator to the end |
|`const_iterator cend() const`      |returns an iterator to the end |
|`reverse_iterator rbegin()`        |returns a reverse iterator to the beginning |
|`const_reverse_iterator rbegin()`  |returns a reverse iterator to the beginning |
|`const_reverse_iterator crbegin()` |returns a reverse iterator to the beginning |
|`reverse_iterator rend()`              |returns a reverse iterator to the end |
|`const_reverse_iterator rend() const`  |returns a reverse iterator to the end |
|`const_reverse_iterator crend() const` |returns a reverse iterator to the end |
|`bool empty() const`                   |checks whether the container is empty |
|`template <typename Key> size_t count(const Key& key)` | returns the number of elements matching specific key |
|`template <typename Key> bool contains(const Key& key) const`       |checks if the container contains element with specific key |
|`std::pair<iterator, bool> insert(const value_type& value)`         |inserts element |
|`template <typename Key> iterator find(const Key& key)`             |finds element with specific key |
|`template <typename Key> const_iterator find(const Key& key) const` |finds element with specific key |
|`template <typename Key> V& at(const Key& key)`                     |access specified element with bounds checking |
|`template <typename Key> const V& at(const Key& key) const`         |access specified element with bounds checking |
|`template <typename Key> V& operator[](const Key& key)`             |access or insert specified element |

MimicMap special member functions:

|Member function                             |Description              |
| ------------------------------------------ | ----------------------- |
|`void reserve(const K& low, const K& high)` |allocate elements        |
|`MimicMap& setLowerLimit(const K& key)`     |set lower limit of keys  |
|`MimicMap& setHigherLimit(const K& key)`    |set higher limit of keys |

Performance comparison
----------------------

`operator[]` random access performance

![`operator[]` performance](https://raw.githubusercontent.com/tadashi9e/MimicMap/main/performance_test/performance_op.png)

`insert` random access performance

![`insert` performance](https://raw.githubusercontent.com/tadashi9e/MimicMap/main/performance_test/performance_insert.png)

`find` random access performance

![`find` performance](https://raw.githubusercontent.com/tadashi9e/MimicMap/main/performance_test/performance_find.png)

License
-------

MIT license.
