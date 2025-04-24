#ifndef INCLUDE_VECTOR_THE_SERENE_HPP_
#define INCLUDE_VECTOR_THE_SERENE_HPP_

#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

template <typename T> class VectorTheSerene {
  private:
    size_t size_;
    size_t capacity;
    T *data;

    void auto_resize(size_t new_size) {
        size_t new_capacity = capacity;
        while (new_size > new_capacity) {
            new_capacity *= 2;
        }
        while (new_size < new_capacity * 2 && new_capacity > 16) {
            new_capacity /= 2;
        }

        unsafe_resize(new_capacity);
    }

    void unsafe_resize(size_t new_capacity) {
        assert(new_capacity >= size_);
        if (new_capacity == capacity) {
            return;
        }

        T *new_data =
            static_cast<T *>(::operator new(sizeof(T) * new_capacity));
        // Use the *true* size here
        for (size_t i = 0; i < size_; ++i) {
            new (&new_data[i]) T(data[i]);
            data[i].~T();
        }
        ::operator delete(data);

        data = new_data;
        capacity = new_capacity;
    }

  public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using reverse_iterator = std::reverse_iterator<T *>;
    using const_reverse_iterator = std::reverse_iterator<const T *>;

    void swap(VectorTheSerene &other) {
        std::swap(size_, other.size_);
        std::swap(capacity, other.capacity);
        std::swap(data, other.data);
    }

    VectorTheSerene() {
        size_ = 0;
        capacity = 16;
        data = static_cast<T *>(::operator new(sizeof(T) * capacity));
    }
    VectorTheSerene(const VectorTheSerene &other) {
        size_ = other.size_;
        capacity = other.capacity;
        data = static_cast<T *>(::operator new(sizeof(T) * capacity));
        for (size_t i = 0; i < size_; ++i) {
            // Can't just assign as this is the raw data
            new (&data[i]) T(other.data[i]);
        }
    }
    VectorTheSerene(VectorTheSerene &&other) { swap(other); }
    VectorTheSerene &operator=(const VectorTheSerene &other) {
        VectorTheSerene tmp(other);
        swap(tmp);
        return *this;
    }

    VectorTheSerene(const T &value, size_t n) {
        size_ = n;
        if (n <= 8) {
            capacity = 16;
        } else {
            capacity = n;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity));
        for (size_t i = 0; i < size_; ++i) {
            new (&data[i]) T(value);
        }
    }
    template <typename Iterator> VectorTheSerene(Iterator begin, Iterator end) {
        size_ = std::distance(begin, end);
        if (size_ <= 8) {
            capacity = 16;
        } else {
            capacity = size_;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity));
        for (size_t i = 0; i < size_; ++i) {
            new (&data[i]) T(*(begin + i));
        }
    }
    VectorTheSerene(std::initializer_list<T> list) {
        size_ = list.size();
        if (size_ <= 8) {
            capacity = 16;
        } else {
            capacity = size_;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity));
        size_t i = 0;
        for (const auto &item : list) {
            new (&data[i++]) T(item);
        }
    }

    VectorTheSerene &operator=(VectorTheSerene &&other) {
        swap(other);
        return *this;
    }
    ~VectorTheSerene() {
        for (size_t i = 0; i < size_; ++i)
            data[i].~T();
        ::operator delete(data);
    }

    T &operator[](size_t index) { return data[index]; }
    const T &operator[](size_t index) const { return data[index]; }
    const T &at(size_t index) const {
        if (index >= size_) { // not < 0, because size_t
            throw std::out_of_range("index out of range");
        }
        return data[index];
    }
    T &at(size_t index) {
        return const_cast<T &>(
            static_cast<const VectorTheSerene<T> *>(this)->at(index));
    }

    void push_back(const T &value) {
        auto_resize(size_ + 1);
        new (&data[size_]) T(value);
        size_++;
    }
    void push_back(T &&value) {
        auto_resize(size_ + 1);
        new (&data[size_]) T(std::move(value));
        size_++;
    }

    void pop_back() {
        if (size_ > 0) {
            size_--;
            data[size_].~T();
            auto_resize(size_);
        }
    }

    T &back() {
        if (size_ == 0) {
            throw std::out_of_range("vector is empty");
        }
        return data[size_ - 1];
    }
    const T &back() const {
        if (size_ == 0) {
            throw std::out_of_range("vector is empty");
        }
        return data[size_ - 1];
    }
    T &front() {
        if (size_ == 0) {
            throw std::out_of_range("vector is empty");
        }
        return data[0];
    }
    const T &front() const {
        if (size_ == 0) {
            throw std::out_of_range("vector is empty");
        }
        return data[0];
    }

    T *begin() { return data; }
    T *end() { return data + size_; }

    const T *begin() const { return data; }
    const T *end() const { return data + size_; }

    const T *cbegin() const { return data; }
    const T *cend() const { return data + size_; }

    std::reverse_iterator<T *> rbegin() {
        return std::reverse_iterator<T *>(end());
    }
    std::reverse_iterator<T *> rend() {
        return std::reverse_iterator<T *>(begin());
    }

    std::reverse_iterator<const T *> rbegin() const {
        return std::reverse_iterator<const T *>(end());
    }
    std::reverse_iterator<const T *> rend() const {
        return std::reverse_iterator<const T *>(begin());
    }

    std::reverse_iterator<const T *> crbegin() const {
        return std::reverse_iterator<const T *>(cend());
    }
    std::reverse_iterator<const T *> crend() const {
        return std::reverse_iterator<const T *>(cbegin());
    }

    size_t size() const { return size_; }

    bool is_empty() const { return size_ == 0; }
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data[i].~T();
        }
        size_ = 0;
        auto_resize(0);
    }
};

template <typename T> void print_vector(const VectorTheSerene<T> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
void print_vector(const VectorTheSerene<VectorTheSerene<T>> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        print_vector(v[i]);
    }
}

template <typename T> using my_vector = VectorTheSerene<T>;

#endif // INCLUDE_VECTOR_THE_SERENE_HPP_
