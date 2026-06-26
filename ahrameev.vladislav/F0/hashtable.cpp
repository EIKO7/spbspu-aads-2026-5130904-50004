#include "hashtable.h"

#include <functional>

namespace ahrameev {

HashTable::HashTable() : capacity_(53), count_(0) {
  table_.resize(capacity_);
}

bool HashTable::insert(const std::string& key, const Record& value) {
  if (load_factor() >= kMaxLoadFactor) {
    return false;
  }
  size_t idx = hash_func(key);
  for (size_t i = 0; i < capacity_; ++i) {
    size_t probe = (idx + i) % capacity_;
    if (!table_[probe].occupied) {
      table_[probe] = {key, value, i, true};
      ++count_;
      return true;
    }
  }
  return false;
}

const Record* HashTable::find(const std::string& key) const {
  size_t idx = hash_func(key);
  for (size_t i = 0; i < capacity_; ++i) {
    size_t probe = (idx + i) % capacity_;
    if (!table_[probe].occupied) {
      return nullptr;
    }
    if (table_[probe].key == key) {
      return &table_[probe].value;
    }
  }
  return nullptr;
}

bool HashTable::erase(const std::string& key) {
  return false;
}

size_t HashTable::size() const {
  return count_;
}

bool HashTable::empty() const {
  return count_ == 0;
}

const std::vector<Entry>& HashTable::entries() const {
  return table_;
}

void HashTable::resize() {}

size_t HashTable::hash_func(const std::string& key) const {
  return std::hash<std::string>{}(key) % capacity_;
}

double HashTable::load_factor() const {
  return static_cast<double>(count_) / capacity_;
}

bool HashTable::is_prime(size_t n) {
  if (n < 2) {
    return false;
  }
  if (n == 2 || n == 3) {
    return true;
  }
  if (n % 2 == 0 || n % 3 == 0) {
    return false;
  }
  for (size_t i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0) {
      return false;
    }
  }
  return true;
}

size_t HashTable::next_prime(size_t n) {
  while (!is_prime(n)) {
    ++n;
  }
  return n;
}

}