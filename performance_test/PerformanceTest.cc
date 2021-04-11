#include "MimicMap.h"
#include <iostream>
#include <random>
#include <unordered_map>
#include <map>
#include <boost/timer/timer.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/unordered_map.hpp>

template<typename T>
T random_key(T from, T to) {
  static std::mt19937_64 rng(0xdeadbeaf);
  std::uniform_int_distribution<T> dist(from, to);
  return static_cast<T>(dist(rng));
}

template<typename M, typename T>
void time_op_random(M& m, T from, T to, int loop) {
  boost::timer::cpu_timer timer;
  for (int i = 0; i < loop; ++i) {
    m[random_key(from, to)] = i;
  }
  std::cout << (static_cast<double>(timer.elapsed().wall) / loop);
}
template<typename M, typename T>
void time_insert_random(M& m, T from, T to, int loop) {
  boost::timer::cpu_timer timer;
  for (int i = 0; i < loop; ++i) {
    const typename M::value_type
      p = std::make_pair(random_key(from, to), i);
    m.insert(p);
  }
  std::cout << (static_cast<double>(timer.elapsed().wall) / loop);
}
template<typename M, typename T>
void time_find_random(M& m, T from, T to, int loop) {
  for (T key = from; key <= to; ++key) {
    const typename M::value_type
      p = std::make_pair(key, key);
    m.insert(p);
  }
  boost::timer::cpu_timer timer;
  for (int i = 0; i < loop; ++i) {
    m.find(random_key(from, to));
  }
  std::cout << (static_cast<double>(timer.elapsed().wall) / loop);
}

std::vector<int>
get_sizes(int min_loop, int max_loop) {
  std::vector<int> counts;
  for(int n = min_loop; n <= max_loop;) {
    counts.push_back(n);
    if (n < 10) {
      ++n;
    } else if (n < 100) {
      n += 10;
    } else if (n < 1000) {
      n += 100;
    } else {
      n += 1000;
    }
  }
  return counts;
}

int main(int argc, char* argv[]) {
  const int from = 0;
  const std::vector<int> sizes = get_sizes(1, 100000);
  const int loop = 1000000;
  {
    std::cerr << "operator[]" << std::endl;
    FILE* fp = freopen("performance_op.dat", "w", stdout);
    std::cout << "# operator[]" << std::endl;
    std::cout << "# MimicMap MimicMap(reserved) unordered_map unordered_map(reserved) flat_map map" << std::endl;
    for (int to : sizes) {
      std::cerr << " " << to << std::endl;
      std::cout << to << " ";
      {
        MimicMap<int, int> m;
        time_op_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        MimicMap<int, int> m;
        m.reserve(from, to);
        time_op_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::unordered_map<int, int> m;
        time_op_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::unordered_map<int, int> m;
        m.reserve(to - from);
        time_op_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        boost::container::flat_map<int, int> m;
        time_op_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::map<int, int> m;
        time_op_random(m, from, to, loop);
      }
      std::cout << std::endl;
    }
    fclose(fp);
    std::cerr << "insert" << std::endl;
    fp = freopen("performance_insert.dat", "w", stdout);
    std::cout << "# insert" << std::endl;
    std::cout << "# MimicMap MimicMap(reserved) unordered_map unordered_map(reserved) flat_map map" << std::endl;
    for (int to : sizes) {
      std::cerr << " " << to << std::endl;
      std::cout << to << " ";
      {
        MimicMap<int, int> m;
        time_insert_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        MimicMap<int, int> m;
        m.reserve(from, to);
        time_insert_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::unordered_map<int, int> m;
        time_insert_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::unordered_map<int, int> m;
        m.reserve(to - from);
        time_insert_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        boost::container::flat_map<int, int> m;
        time_insert_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::map<int, int> m;
        time_insert_random(m, from, to, loop);
      }
      std::cout << std::endl;
    }
    fclose(fp);
    std::cerr << "find" << std::endl;
    fp = freopen("performance_find.dat", "w", stdout);
    std::cout << "# find" << std::endl;
    std::cout << "# MimicMap unordered_map flat_map map" << std::endl;
    for (int to : sizes) {
      std::cerr << " " << to << std::endl;
      std::cout << to << " ";
      {
        MimicMap<int, int> m;
        m.reserve(from, to);
        time_find_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::unordered_map<int, int> m;
        m.reserve(to - from);
        time_find_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        boost::container::flat_map<int, int> m;
        time_find_random(m, from, to, loop);
      }
      std::cout << " ";
      {
        std::map<int, int> m;
        time_find_random(m, from, to, loop);
      }
      std::cout << std::endl;
    }
    fclose(fp);
  }
}
