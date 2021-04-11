// -*- mode:c++;coding:utf-8 -*-
#ifndef _RIGIDMAP_H_
#define _RIGIDMAP_H_

#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * the sorted associative container based on fixed size std::vector.
 */
template<typename K, typename V>
class RigidMap {
 public:
  using value_type = std::pair<K, V>;
  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;
  using allocator_type = typename std::vector<value_type>::allocator_type;
  /**
   * constructs an empty container.
   */
  RigidMap(const allocator_type& allocator = allocator_type())
    : elements(allocator) {}
  /**
   * copy constructor.
   */
  RigidMap(const RigidMap<K, V>& orig) : elements(orig.elements) {}
  /**
   * constructs with the contents of the range [first, last).
   */
  template<typename IT>
  RigidMap(IT first, IT last,
           const allocator_type& allocator = allocator_type())
    : elements(first, last, allocator) {
  }
  /**
   * copy assign operator.
   */
  RigidMap<K, V>& operator=(const RigidMap<K, V>& orig) {
    elements = orig.elements;
    return *this;
  }
  /**
   * swaps the contents.
   */
  void swap(RigidMap<K, V>& other) {
    std::swap(elements, other.elements);
  }
  /**
   * returns the number of elements.
   * same as the number from lower limit of keys to maximum limit of keys.
   */
  size_t size() const {
    return elements.size();
  }
  /**
   * allocate elements.
   */
  void reserve(const K& low,
               const K& high) {
    if (elements.empty()) {
      for (K i = low; i <= high; ++i) {
        // elements.push_back(std::make_pair(i, V()));
        elements.emplace_back(i, V());
      }
      return;
    }
    const K minKey = elements.begin()->first;
    if (minKey > low) {
      dig(low);
    }
    const K maxKey = elements.rbegin()->first;
    if (maxKey < high) {
      pile(high);
    }
  }
  /**
   * set lower limit of keys.
   */
  RigidMap& setLowerLimit(const K& key) {
    if (elements.empty()) {
      // elements.push_back(std::make_pair(key, V()));
      elements.emplace_back(key, V());
      assert(elements.begin()->first == key);
      return *this;
    }
    const K maxKey = elements.rbegin()->first;
    if (maxKey < key) {
      elements.clear();
      // elements.push_back(std::make_pair(key, V()));
      elements.emplace_back(key, V());
      assert(elements.begin()->first == key);
      return *this;
    }
    const K minKey = elements.begin()->first;
    if (minKey < key) {
      elements.erase(elements.begin(),
                     elements.begin() + (key - minKey));
    } else if (minKey > key) {
      dig(key);
    }
    assert(elements.begin()->first == key);
    return *this;
  }
  /**
   * set higher limit of keys.
   */
  RigidMap& setHigherLimit(const K& key) {
    if (elements.empty()) {
      // elements.push_back(std::make_pair(key, V()));
      elements.emplace_back(key, V());
      assert(elements.rbegin()->first == key);
      return *this;
    }
    const K minKey = elements.begin()->first;
    if (minKey > key) {
      elements.clear();
      // elements.push_back(std::make_pair(key, V()));
      elements.emplace_back(key, V());
      assert(elements.rbegin()->first == key);
      return *this;
    }
    const K maxKey = elements.rbegin()->first;
    if (maxKey > key) {
      elements.resize(key - minKey + 1);
    } else if (maxKey < key) {
      pile(key);
    }
    assert(elements.rbegin()->first == key);
    return *this;
  }
  /**
   * clears the contents.
   */
  void clear() {
    elements.clear();
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() {
    return elements.begin();
  }
  /**
   * returns an iterator to the beginning.
   */
  const_iterator begin() const {
    return cbegin();
  }
  /**
   * returns an iterator to the beginning.
   */
  const_iterator cbegin() const {
    return elements.begin();
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() {
    return elements.end();
  }
  /**
   * returns an iterator to the end.
   */
  const_iterator end() const {
    return cend();
  }
  /**
   * returns an iterator to the end.
   */
  const_iterator cend() const {
    return elements.end();
  }
  using reverse_iterator =
    typename std::vector<value_type>::reverse_iterator;
  using const_reverse_iterator =
    typename std::vector<value_type>::const_reverse_iterator;
  /**
   * returns a reverse iterator to the beginning.
   */
  reverse_iterator rbegin() {
    return elements.rbegin();
  }
  /**
   * returns a reverse iterator to the beginning.
   */
  const_reverse_iterator rbegin() const {
    return crbegin();
  }
  /**
   * returns a reverse iterator to the beginning.
   */
  const_reverse_iterator crbegin() const {
    return elements.rbegin();
  }
  /**
   * returns a reverse iterator to the end.
   */
  reverse_iterator rend() {
    return elements.rend();
  }
  /**
   * returns a reverse iterator to the end.
   */
  const_reverse_iterator rend() const {
    return crend();
  }
  /**
   * returns a reverse iterator to the end.
   */
  const_reverse_iterator crend() const {
    return elements.rend();
  }
  /**
   * checks whether the container is empty.
   */
  bool empty() const {
    return elements.empty();
  }
  /**
   * returns the number of elements matching specific key.
   */
  template <typename Key>
  size_t count(const Key& key) {
    return contains(key) ? 1 : 0;
  }
  /**
   * checks if the container contains element with specific key.
   */
  template <typename Key>
  bool contains(const Key& key) const {
    if (elements.empty()) {
      return false;
    }
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      return false;
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      return false;
    }
    return true;
  }
  /**
   * assigns to the current element.
   *
   * returns a pair consisting of an iterator to the element
   * and a bool false.
   *
   * exceptions:
   *   std::out_of_range if key exceeds limits.
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    if (elements.empty()) {
      throw std::out_of_range("empty map");
    }
    const K key = value.first;
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      throw std::out_of_range("lower limit exceeded");
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      throw std::out_of_range("higher limit exceeded");
    }
    iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    iter->second = value.second;
    return std::make_pair(iter, false);
  }
  /**
   * finds element with specific key.
   */
  template <typename Key>
  iterator find(const Key& key) {
    if (elements.empty()) {
      return end();
    }
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      return end();
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      return end();
    }
    iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    return iter;
  }
  /**
   * finds element with specific key.
   */
  template <typename Key>
  const_iterator find(const Key& key) const {
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      return end();
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      return end();
    }
    const_iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    return iter;
  }
  /**
   * finds element with specific key.
   *
   * exceptions:
   *   std::out_of_range if key not found.
   */
  template <typename Key>
  V& at(const Key& key) {
    iterator iter = find(key);
    if (iter == end()) {
      throw std::out_of_range("key not found");
    }
    assert(iter->first == key);
    return iter->second;
  }
  /**
   * access specified element with bounds checking.
   *
   * exceptions:
   *   std::out_of_range if key not found.
   */
  template <typename Key>
  const V& at(const Key& key) const {
    const_iterator iter = find(key);
    if (iter == end()) {
      throw std::out_of_range("key not found");
    }
    assert(iter->first == key);
    return iter->second;
  }
  /**
   * access or insert specified element.
   *
   * exceptions:
   *   std::out_of_range if key exceeds limits.
   */
  template <typename Key>
  V& operator[](const Key& key) {
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      throw std::out_of_range("lower limit exceeded");
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      throw std::out_of_range("higher limit exceeded");
    }
    iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    return iter->second;
  }

 private:
  std::vector<value_type> elements;
  /**
   * expand the region of elements forward given key.
   */
  void dig(const K& key) {
    std::vector<value_type> elements2;
    const K minKey = elements.begin()->first;
    elements2.reserve(elements.size() + minKey - key);
    for (K i = key; i < minKey; ++i) {
      // elements2.push_back(std::make_pair(i, V()));
      elements2.emplace_back(i, V());
    }
    for (iterator
           iter = elements.begin(); iter != elements.end(); ++iter) {
      // elements2.push_back(*iter);
      elements2.emplace_back(*iter);
    }
    std::swap(elements, elements2);
  }
  /**
   * expand the region of elements toward given key.
   */
  void pile(const K& key) {
    const K maxKey = elements.rbegin()->first;
    for (K i = maxKey + 1; i <= key; ++i) {
      // elements.push_back(std::make_pair(i, V()));
      elements.emplace_back(i, V());
    }
  }
};
#endif  // _RIGIDMAP_H_
