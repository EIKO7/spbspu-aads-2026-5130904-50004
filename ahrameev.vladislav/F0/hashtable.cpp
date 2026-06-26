// src/hashtable.cpp
#include "hashtable.h"

#include <functional>

namespace ahrameev {

HashTable::HashTable() : capacity_(53), count_(0) {
  table_.resize(capacity_);
}

bool HashTable::insert(const std::string& key, const Record& value) {
  return false;
}

const Record* HashTable::find(const std::string& key) const {
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
  return n > 1;
}

size_t HashTable::next_prime(size_t n) {
  return n;
}

}