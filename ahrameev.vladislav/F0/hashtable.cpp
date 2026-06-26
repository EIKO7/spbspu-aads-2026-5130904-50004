#include "hashtable.h"

#include <functional>
#include <utility>

namespace ahrameev {

HashTable::HashTable() : capacity_(53), count_(0) {
  table_.resize(capacity_);
}

bool HashTable::insert(const std::string& key, const Record& value) {
  if (load_factor() >= kMaxLoadFactor) {
    resize();
  }
  size_t ideal = hash_func(key);
  Entry new_entry{key, value, 0, true};

  for (size_t i = 0; i < capacity_; ++i) {
    size_t idx = (ideal + i) % capacity_;
    if (!table_[idx].occupied) {
      table_[idx] = std::move(new_entry);
      table_[idx].distance_from_ideal = i;
      ++count_;
      return true;
    }
    if (table_[idx].key == key) {
      return false;
    }
    if (i > table_[idx].distance_from_ideal) {
      std::swap(new_entry, table_[idx]);
      size_t ideal_displaced = hash_func(new_entry.key);
      new_entry.distance_from_ideal =
          (idx - ideal_displaced + capacity_) % capacity_;
    }
  }
  return false;
}

const Record* HashTable::find(const std::string& key) const {
  size_t ideal = hash_func(key);
  for (size_t i = 0; i < capacity_; ++i) {
    size_t idx = (ideal + i) % capacity_;
    if (!table_[idx].occupied) {
      return nullptr;
    }
    if (table_[idx].key == key) {
      return &table_[idx].value;
    }
    if (table_[idx].distance_from_ideal < i) {
      return nullptr;
    }
  }
  return nullptr;
}

bool HashTable::erase(const std::string& key) {
  size_t ideal = hash_func(key);
  for (size_t i = 0; i < capacity_; ++i) {
    size_t idx = (ideal + i) % capacity_;
    if (!table_[idx].occupied) {
      return false;
    }
    if (table_[idx].key == key) {
      size_t shift = idx;
      while (true) {
        size_t next = (shift + 1) % capacity_;
        if (!table_[next].occupied ||
            table_[next].distance_from_ideal == 0) {
          table_[shift].occupied = false;
          table_[shift].key.clear();
          table_[shift].value = Record();
          table_[shift].distance_from_ideal = 0;
          break;
        }
        table_[shift] = std::move(table_[next]);
        table_[shift].distance_from_ideal--;
        shift = next;
      }
      --count_;
      return true;
    }
    if (table_[idx].distance_from_ideal < i) {
      return false;
    }
  }
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

void HashTable::resize() {
  size_t new_cap = next_prime(capacity_ * 2);
  std::vector<Entry> old = std::move(table_);
  capacity_ = new_cap;
  table_.assign(capacity_, Entry());
  count_ = 0;
  for (const auto& e : old) {
    if (e.occupied) {
      insert(e.key, e.value);
    }
  }
}

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