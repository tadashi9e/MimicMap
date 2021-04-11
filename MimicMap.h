// -*- mode:c++;coding:utf-8 -*-
// Copyright 2021 tadashi9@gmail.com
#ifndef MIMICMAP_H_
#define MIMICMAP_H_
#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

/**
 * the sorted associative container based on std::vector.
 */
template<typename K, typename V>
class MimicMap {
 public:
  using key_type = K;
  using mapped_type = V;
  using value_type = std::pair<K, V>;
  using iterator = typename std::vector<value_type>::iterator;
  using const_iterator = typename std::vector<value_type>::const_iterator;
  using allocator_type = typename std::vector<value_type>::allocator_type;
  /**
   * constructs an empty container.
   */
  explicit MimicMap(const allocator_type& allocator = allocator_type())
    : elements(allocator), hasLowerLimit(false), hasHigherLimit(false) {}
  /**
   * copy constructor.
   */
  MimicMap(const MimicMap<K, V>& orig)
    : elements(orig.elements),
      hasLowerLimit(orig.hasLowerLimit),
      hasHigherLimit(orig.hasHigherLimit),
      lowerLimit(orig.lowerLimit),
      higherLimit(orig.higherLimit) {
  }
  /**
   * constructs with the contents of the range [first, last).
   */
  template<typename IT>
  MimicMap(IT first, IT last,
           const allocator_type& allocator = allocator_type())
    : elements(first, last, allocator),
      hasLowerLimit(false), hasHigherLimit(false) {
  }
  /**
   * copy assign operator.
   */
  MimicMap<K, V>& operator=(const MimicMap<K, V>& orig) {
    elements = orig.elements;
    hasLowerLimit = orig.hasLowerLimit;
    hasHigherLimit = orig.hasHigherLimit;
    lowerLimit = orig.lowerLimit;
    higherLimit = orig.higherLimit;
  }
  /**
   * swaps the contents.
   */
  void swap(MimicMap<K, V>& other) {
    std::swap(elements, other.elements);
    std::swap(hasLowerLimit, other.hasLowerLimit);
    std::swap(hasHigherLimit, other.hasHigherLimit);
    std::swap(lowerLimit, other.lowerLimit);
    std::swap(higherLimit, other.higherLimit);
  }
  /**
   * returns the number of elements.
   * same as the number from minimum key to maximum key.
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
   * removes lower key entries if exists.
   */
  MimicMap& setLowerLimit(const K& key) {
    this->lowerLimit = key;
    this->hasLowerLimit = true;
    if (elements.empty()) {
      return *this;
    }
    const K maxKey = elements.rbegin()->first;
    if (maxKey < key) {
      elements.clear();
      return *this;
    }
    const K minKey = elements.begin()->first;
    if (minKey < key) {
      elements.erase(elements.begin(),
                     elements.begin() + (key - minKey));
      assert(elements.begin()->first == key);
    }
    return *this;
  }
  /**
   * set higher limit of keys.
   * removes higher key entries if exists.
   */
  MimicMap& setHigherLimit(const K& key) {
    this->higherLimit = key;
    this->hasHigherLimit = true;
    if (elements.empty()) {
      return *this;
    }
    const K minKey = elements.begin()->first;
    if (minKey > key) {
      elements.clear();
      return *this;
    }
    const K maxKey = elements.rbegin()->first;
    if (maxKey > key) {
      elements.resize(key - minKey + 1);
      assert(elements.rbegin()->first == key);
    }
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
   * inserts element.
   *
   * returns a pair consisting of an iterator to the element
   * (inserted or updated) and a bool denoting
   * whether inserted or not.
   *
   * exceptions:
   *   std::out_of_range if key exceeds limits.
   */
  std::pair<iterator, bool> insert(const value_type& value) {
    const K key = value.first;
    if (elements.empty()) {
      // elements.push_back(std::make_pair(key, value.second));
      elements.emplace_back(key, value.second);
      return std::make_pair(begin(), true);
    }
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      dig(key);
      assert(elements.begin()->first == key);
      elements.begin()->second = value.second;
      return std::make_pair(begin(), true);
    }
    const K maxKey = elements.rbegin()->first;
    bool inserted = false;
    if (key > maxKey) {
      pile(key);
      assert(elements.rbegin()->first == key);
      inserted = true;
    }
    iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    iter->second = value.second;
    return std::make_pair(iter, inserted);
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
   * access specified element with bounds checking.
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
    if (elements.empty()) {
      // elements.push_back(std::make_pair(key, V()));
      elements.emplace_back(key, V());
      return elements.begin()->second;
    }
    const K minKey = elements.begin()->first;
    if (key < minKey) {
      dig(key);
      assert(elements.begin()->first == key);
      return elements.begin()->second;
    }
    const K maxKey = elements.rbegin()->first;
    if (key > maxKey) {
      pile(key);
    }
    iterator iter = elements.begin() + (key - minKey);
    assert(iter->first == key);
    return iter->second;
  }

 private:
  /**
   * wrapped std::vector
   */
  std::vector<value_type> elements;
  bool hasLowerLimit;
  bool hasHigherLimit;
  K lowerLimit;
  K higherLimit;
  /**
   * expand the region of elements forward given key.
   */
  void dig(const K& key) {
    if (hasLowerLimit && key < lowerLimit) {
      throw std::out_of_range("lower limit exceeded");
    }
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
    if (hasHigherLimit && key > higherLimit) {
      throw std::out_of_range("higher limit exceeded");
    }
    const K maxKey = elements.rbegin()->first;
    for (K i = maxKey + 1; i <= key; ++i) {
      // elements.push_back(std::make_pair(i, V()));
      elements.emplace_back(i, V());
    }
  }
};
#endif  // MIMICMAP_H_
