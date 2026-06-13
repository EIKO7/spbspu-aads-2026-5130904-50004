#ifndef AHRAMEEV_VECTOR_H
#define AHRAMEEV_VECTOR_H

#include <cstddef>
#include <utility>
#include <algorithm>

namespace ahrameev {

template <typename T>
class Vector
{
private:
    T* data_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

    void reallocate(size_t cap)
    {
        T* nd = new T[cap];
        for (size_t i = 0; i < size_; ++i) {
            nd[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = nd;
        capacity_ = cap;
    }

public:
    Vector() = default;

    ~Vector()
    {
        delete[] data_;
    }

    Vector(const Vector& o)
    {
        if (o.size_ > 0) {
            data_ = new T[o.capacity_];
            capacity_ = o.capacity_;
            for (size_t i = 0; i < o.size_; ++i) {
                data_[i] = o.data_[i];
            }
            size_ = o.size_;
        }
    }

    Vector(Vector&& o) noexcept
        : data_(o.data_), size_(o.size_), capacity_(o.capacity_)
    {
        o.data_ = nullptr;
        o.size_ = 0;
        o.capacity_ = 0;
    }

    Vector& operator=(const Vector& o)
    {
        if (this != &o) {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            if (o.size_ > 0) {
                data_ = new T[o.capacity_];
                capacity_ = o.capacity_;
                for (size_t i = 0; i < o.size_; ++i) {
                    data_[i] = o.data_[i];
                }
                size_ = o.size_;
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& o) noexcept
    {
        if (this != &o) {
            delete[] data_;
            data_ = o.data_;
            size_ = o.size_;
            capacity_ = o.capacity_;
            o.data_ = nullptr;
            o.size_ = 0;
            o.capacity_ = 0;
        }
        return *this;
    }

    void push_back(const T& v)
    {
        if (size_ == capacity_) {
            reallocate(capacity_ > 0 ? capacity_ * 2 : 4);
        }
        data_[size_] = v;
        ++size_;
    }

    void push_back(T&& v)
    {
        if (size_ == capacity_) {
            reallocate(capacity_ > 0 ? capacity_ * 2 : 4);
        }
        data_[size_] = std::move(v);
        ++size_;
    }

    bool contains(const T& v) const
    {
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] == v) {
                return true;
            }
        }
        return false;
    }

    bool erase_value(const T& v)
    {
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] == v) {
                for (size_t j = i; j < size_ - 1; ++j) {
                    data_[j] = std::move(data_[j + 1]);
                }
                --size_;
                return true;
            }
        }
        return false;
    }

    T& operator[](size_t i)
    {
        return data_[i];
    }

    const T& operator[](size_t i) const
    {
        return data_[i];
    }

    size_t size() const
    {
        return size_;
    }

    bool empty() const
    {
        return size_ == 0;
    }

    T* begin()
    {
        return data_;
    }

    T* end()
    {
        return data_ + size_;
    }

    const T* begin() const
    {
        return data_;
    }

    const T* end() const
    {
        return data_ + size_;
    }

    void sort()
    {
        if (size_ > 1) {
            std::sort(data_, data_ + size_);
        }
    }
};

}
#endif 