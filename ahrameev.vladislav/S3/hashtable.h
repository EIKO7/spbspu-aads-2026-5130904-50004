#ifndef AHRAMEEV_HASHTABLE_H
#define AHRAMEEV_HASHTABLE_H

#include <string>
#include <cstdint>
#include <utility>
#include <stdexcept>
#include "vector.h"

namespace ahrameev {

class SipHash {
    uint64_t k0_, k1_;
    std::string buffer_;

    static uint64_t rotl(uint64_t x, int b) { return (x << b) | (x >> (64 - b)); }
    static void sipround(uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3) {
        v0 += v1; v1 = rotl(v1, 13); v1 ^= v0; v0 = rotl(v0, 32);
        v2 += v3; v3 = rotl(v3, 16); v3 ^= v2;
        v0 += v3; v3 = rotl(v3, 21); v3 ^= v0;
        v2 += v1; v1 = rotl(v1, 17); v1 ^= v2; v2 = rotl(v2, 32);
    }

    uint64_t compute(const uint8_t* in, size_t len) const {
        uint64_t v0 = 0x736f6d6570736575ULL ^ k0_, v1 = 0x646f72616e646f6dULL ^ k1_;
        uint64_t v2 = 0x6c7967656e657261ULL ^ k0_, v3 = 0x7465646279746573ULL ^ k1_;
        const uint8_t* end = in + len - (len % 8);
        uint64_t b = ((uint64_t)len) << 56;
        for (; in != end; in += 8) {
            uint64_t m = ((uint64_t)in[7]<<56)|((uint64_t)in[6]<<48)|((uint64_t)in[5]<<40)|((uint64_t)in[4]<<32)|
                         ((uint64_t)in[3]<<24)|((uint64_t)in[2]<<16)|((uint64_t)in[1]<<8)|(uint64_t)in[0];
            v3 ^= m; sipround(v0, v1, v2, v3); sipround(v0, v1, v2, v3); v0 ^= m;
        }
        int left = len & 7;
        if (left >= 7) b |= ((uint64_t)in[6]) << 48;
        if (left >= 6) b |= ((uint64_t)in[5]) << 40;
        if (left >= 5) b |= ((uint64_t)in[4]) << 32;
        if (left >= 4) b |= ((uint64_t)in[3]) << 24;
        if (left >= 3) b |= ((uint64_t)in[2]) << 16;
        if (left >= 2) b |= ((uint64_t)in[1]) << 8;
        if (left >= 1) b |= ((uint64_t)in[0]);
        v3 ^= b; sipround(v0, v1, v2, v3); sipround(v0, v1, v2, v3); v0 ^= b; v2 ^= 0xff;
        sipround(v0, v1, v2, v3); sipround(v0, v1, v2, v3); sipround(v0, v1, v2, v3); sipround(v0, v1, v2, v3);
        return v0 ^ v1 ^ v2 ^ v3;
    }

public:
    using result_type = uint64_t;

    SipHash() : k0_(0), k1_(0) {}
    explicit SipHash(uint64_t seed) : k0_(seed), k1_(0) {}
    SipHash(const void* p, size_t n) : k0_(0), k1_(0) {
        if (n == 16) {
            const uint8_t* b = reinterpret_cast<const uint8_t*>(p);
            k0_ = ((uint64_t)b[7]<<56)|((uint64_t)b[6]<<48)|((uint64_t)b[5]<<40)|((uint64_t)b[4]<<32)|
                  ((uint64_t)b[3]<<24)|((uint64_t)b[2]<<16)|((uint64_t)b[1]<<8)|(uint64_t)b[0];
            k1_ = ((uint64_t)b[15]<<56)|((uint64_t)b[14]<<48)|((uint64_t)b[13]<<40)|((uint64_t)b[12]<<32)|
                  ((uint64_t)b[11]<<24)|((uint64_t)b[10]<<16)|((uint64_t)b[9]<<8)|(uint64_t)b[8];
        } else if (n > 0) {
            update(p, n);
            result();
        }
    }

    void update(const void* p, size_t n) { buffer_.append(reinterpret_cast<const char*>(p), n); }
    uint64_t result() {
        uint64_t h = compute(reinterpret_cast<const uint8_t*>(buffer_.data()), buffer_.size());
        buffer_.clear();
        return h;
    }

    uint64_t operator()(const std::string& data) {
        buffer_.clear(); update(data.data(), data.size()); return result();
    }

    uint64_t operator()(const std::pair<std::string, std::string>& p) {
        buffer_.clear();
        uint32_t l1 = p.first.size(), l2 = p.second.size();
        update(&l1, sizeof(l1)); update(p.first.data(), l1);
        update(&l2, sizeof(l2)); update(p.second.data(), l2);
        return result();
    }
};

struct StrEq { bool operator()(const std::string& a, const std::string& b) const { return a == b; } };
struct PairEq {
    bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) const {
        return a.first == b.first && a.second == b.second;
    }
};

template <class Key, class Value, class Hash, class Equal>
class HashTable {
    enum class State { Empty, Occupied, Tombstone };
    struct Slot { State state; Key key; Value value; Slot() : state(State::Empty) {} };

    Slot* table_;
    size_t capacity_, size_, tombstones_;
    Hash hash_func_;
    Equal equal_func_;

    size_t probe(size_t hash, size_t i) const { return (hash + i * i) % capacity_; }

    void reallocate(size_t new_cap) {
        Slot* old = table_; size_t old_cap = capacity_;
        table_ = new Slot[new_cap]; capacity_ = new_cap; size_ = 0; tombstones_ = 0;
        for (size_t i = 0; i < old_cap; ++i) {
            if (old[i].state == State::Occupied) {
                size_t h = hash_func_(old[i].key);
                for (size_t j = 0; ; ++j) {
                    size_t idx = probe(h, j);
                    if (table_[idx].state == State::Empty) {
                        table_[idx].state = State::Occupied;
                        table_[idx].key = std::move(old[i].key);
                        table_[idx].value = std::move(old[i].value);
                        size_++; break;
                    }
                }
            }
        }
        delete[] old;
    }

public:
    explicit HashTable(size_t cap = 16) : capacity_(cap), size_(0), tombstones_(0) { table_ = new Slot[capacity_]; }
    ~HashTable() { delete[] table_; }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&& o) noexcept : table_(o.table_), capacity_(o.capacity_), size_(o.size_), tombstones_(o.tombstones_), hash_func_(std::move(o.hash_func_)), equal_func_(std::move(o.equal_func_)) {
        o.table_ = nullptr; o.capacity_ = o.size_ = o.tombstones_ = 0;
    }

    void add(const Key& k, const Value& v) {
        size_t h = hash_func_(k);
        int first_ts = -1;
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(h, i);
            if (table_[idx].state == State::Empty) {
                size_t ins = (first_ts != -1) ? (size_t)first_ts : idx;
                if (first_ts != -1) tombstones_--;
                table_[ins].state = State::Occupied; table_[ins].key = k; table_[ins].value = v; size_++; return;
            }
            if (table_[idx].state == State::Occupied && equal_func_(table_[idx].key, k)) throw std::invalid_argument("Key exists");
            if (table_[idx].state == State::Tombstone && first_ts == -1) first_ts = (int)idx;
        }
        if (first_ts != -1) {
            size_t ins = (size_t)first_ts; tombstones_--;
            table_[ins].state = State::Occupied; table_[ins].key = k; table_[ins].value = v; size_++; return;
        }
        throw std::length_error("Hash table is full");
    }

    Value& get(const Key& k) {
        size_t h = hash_func_(k);
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(h, i);
            if (table_[idx].state == State::Empty) break;
            if (table_[idx].state == State::Occupied && equal_func_(table_[idx].key, k)) return table_[idx].value;
        }
        throw std::out_of_range("Key not found");
    }

    bool has(const Key& k) const {
        size_t h = hash_func_(k);
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(h, i);
            if (table_[idx].state == State::Empty) return false;
            if (table_[idx].state == State::Occupied && equal_func_(table_[idx].key, k)) return true;
        }
        return false;
    }

    Value drop(const Key& k) {
        size_t h = hash_func_(k);
        for (size_t i = 0; i < capacity_; ++i) {
            size_t idx = probe(h, i);
            if (table_[idx].state == State::Empty) break;
            if (table_[idx].state == State::Occupied && equal_func_(table_[idx].key, k)) {
                Value v = std::move(table_[idx].value);
                table_[idx].state = State::Tombstone; size_--; tombstones_++; return v;
            }
        }
        throw std::out_of_range("Key not found");
    }

    void rehash(size_t slots) {
        if (slots <= size_) throw std::invalid_argument("New capacity too small");
        reallocate(slots);
    }

    struct Iterator {
        Slot* ptr, *end;
        Iterator& operator++() { do { ++ptr; } while (ptr != end && ptr->state != State::Occupied); return *this; }
        bool operator!=(const Iterator& o) const { return ptr != o.ptr; }
        Slot& operator*() { return *ptr; }
        Slot* operator->() { return ptr; }
    };
    Iterator begin() { Iterator it{table_, table_ + capacity_}; if (it.ptr != it.end && it.ptr->state != State::Occupied) ++it; return it; }
    Iterator end() { return Iterator{table_ + capacity_, table_ + capacity_}; }
};

} 
#endif 