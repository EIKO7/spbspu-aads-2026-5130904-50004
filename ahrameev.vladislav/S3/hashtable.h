#ifndef AHFRAMEEV_HASHTABLE_H
#define AHFRAMEEV_HASHTABLE_H

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

} 
#endif