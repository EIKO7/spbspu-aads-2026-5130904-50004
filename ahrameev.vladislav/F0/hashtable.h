#ifndef AHFRAMEEV_HASHTABLE_H
#define AHFRAMEEV_HASHTABLE_H

#include <string>
#include <vector>

namespace ahrameev {

struct Record {
  std::string login;
  std::string password;
};

struct Entry {
  std::string key;
  Record value;
  size_t distance_from_ideal;
  bool occupied;
};

class HashTable {
 public:
  HashTable();
  bool insert(const std::string& key, const Record& value);
  const Record* find(const std::string& key) const;
  bool erase(const std::string& key);
  size_t size() const;
  bool empty() const;
  const std::vector<Entry>& entries() const;

 private:
  std::vector<Entry> table_;
  size_t capacity_;
  size_t count_;
  static constexpr double k_max_load_factor = 0.7;

  void resize();
  size_t hash_func(const std::string& key) const;
  double load_factor() const;
  static bool is_prime(size_t n);
  static size_t next_prime(size_t n);
};

}

#endif