#ifndef AHFRAMEEV_VECTOR_H
#define AHFRAMEEV_VECTOR_H

#include <cstddef>
#include <utility>
#include <algorithm>

namespace ahrameev {

template <typename T>
class Vector {
    T* data_ = nullptr;
    size_t size_ = 0, capacity_ = 0;

    void reallocate(size_t cap) {
        T* nd = new T[cap];
        for (size_t i = 0; i < size_; ++i) nd[i] = std::move(data_[i]);
        delete[] data_;
        data_ = nd;
        capacity_ = cap;
    }

public:
    Vector() = default;
    ~Vector() { delete[] data_; }

    Vector(const Vector& o) {
        if (o.size_) {
            data_ = new T[o.capacity_];
            capacity_ = o.capacity_;
            for (size_t i = 0; i < o.size_; ++i) data_[i] = o.data_[i];
            size_ = o.size_;
        }
    }

    Vector(Vector&& o) noexcept : data_(o.data_), size_(o.size_), capacity_(o.capacity_) {
        o.data_ = nullptr; o.size_ = o.capacity_ = 0;
    }

    Vector& operator=(const Vector& o) {
        if (this != &o) {
            delete[] data_; data_ = nullptr; size_ = capacity_ = 0;
            if (o.size_) {
                data_ = new T[o.capacity_]; capacity_ = o.capacity_;
                for (size_t i = 0; i < o.size_; ++i) data_[i] = o.data_[i];
                size_ = o.size_;
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& o) noexcept {
        if (this != &o) {
            delete[] data_;
            data_ = o.data_; size_ = o.size_; capacity_ = o.capacity_;
            o.data_ = nullptr; o.size_ = o.capacity_ = 0;
        }
        return *this;
    }
};

} 
#endif 