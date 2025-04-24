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
    size_t capacity_;
    T *data;

    void auto_resize(size_t new_size) {
        size_t new_capacity = capacity_;
        while (new_size > new_capacity) {
            new_capacity *= 2;
        }
        // And we never auto-shrink

        unsafe_resize(new_capacity);
    }

    void unsafe_resize(size_t new_capacity) {
        assert(new_capacity >= size_);
        if (new_capacity == capacity_) {
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
        capacity_ = new_capacity;
    }

  public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using reverse_iterator = std::reverse_iterator<T *>;
    using const_reverse_iterator = std::reverse_iterator<const T *>;

    void swap(VectorTheSerene &other) {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data, other.data);
    }

    VectorTheSerene() {
        size_ = 0;
        capacity_ = 16;
        data = static_cast<T *>(::operator new(sizeof(T) * capacity_));
    }
    VectorTheSerene(const VectorTheSerene &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data = static_cast<T *>(::operator new(sizeof(T) * capacity_));
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
            capacity_ = 16;
        } else {
            capacity_ = n;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity_));
        for (size_t i = 0; i < size_; ++i) {
            new (&data[i]) T(value);
        }
    }
    template <typename Iterator> VectorTheSerene(Iterator begin, Iterator end) {
        size_ = std::distance(begin, end);
        if (size_ <= 8) {
            capacity_ = 16;
        } else {
            capacity_ = size_;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity_));
        for (size_t i = 0; i < size_; ++i) {
            new (&data[i]) T(*(begin + i));
        }
    }
    VectorTheSerene(std::initializer_list<T> list) {
        size_ = list.size();
        if (size_ <= 8) {
            capacity_ = 16;
        } else {
            capacity_ = size_;
        }
        data = static_cast<T *>(::operator new(sizeof(T) * capacity_));
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
        }
    }

    template <typename... Args> T &emplace_back(Args &&...args) {
        auto_resize(size_ + 1);
        new (&data[size_]) T(std::forward<Args>(args)...);
        size_++;
        return data[size_ - 1];
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
    size_t capacity() const { return capacity_; }

    bool is_empty() const { return size_ == 0; }
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data[i].~T();
        }
        size_ = 0;
        auto_resize(0);
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            unsafe_resize(new_capacity);
        }
    }
    void shrink_to_fit() {
        if (size_ < capacity_) {
            unsafe_resize(size_);
        }
    }

    void resize(size_t new_size) {
        // Remove all the items >=new_size
        for (size_t i = new_size; i < size_; ++i) {
            data[i].~T();
        }
        auto_resize(new_size);
        // Add new items
        for (size_t i = size_; i < new_size; ++i) {
            new (&data[i]) T();
        }
        size_ = new_size;
    }
    void resize(size_t new_size, const T &value) {
        // Remove all the items >=new_size
        for (size_t i = new_size; i < size_; ++i) {
            data[i].~T();
        }
        auto_resize(new_size);
        // Add new items
        for (size_t i = size_; i < new_size; ++i) {
            new (&data[i]) T(value);
        }
        size_ = new_size;
    }

    auto insert(size_t index, const T &value) {
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }
        auto_resize(size_ + 1);
        for (size_t i = size_ - 1; i > index; --i) {
            new (&data[i]) T(std::move(data[i - 1]));
            data[i - 1].~T();
        }
        new (&data[index]) T(value);
        size_++;
        return data + index;
    }

    auto insert(size_t index, T &&value) {
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }
        auto_resize(size_ + 1);
        for (size_t i = size_ - 1; i > index; --i) {
            new (&data[i]) T(std::move(data[i - 1]));
            data[i - 1].~T();
        }
        new (&data[index]) T(std::move(value));
        size_++;
        return data + index;
    }

    template <typename Iterator>
    auto insert(size_t index, Iterator begin, Iterator end) {
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }
        size_t count = std::distance(begin, end);
        auto_resize(size_ + count);
        for (size_t i = size_ - 1; i > index; --i) {
            new (&data[i]) T(std::move(data[i - count]));
            data[i - count].~T();
        }
        for (size_t i = 0; i < count; ++i) {
            new (&data[index + i]) T(*(begin + i));
        }
        size_ += count;
        return data + index;
    }

    auto erase(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        data[index].~T();
        for (size_t i = index + 1; i < size_; ++i) {
            new (&data[i - 1]) T(std::move(data[i]));
            data[i].~T();
        }
        size_--;
        return data + index;
    }

    auto erase(size_t begin, size_t end) {
        if (begin >= size_ || end > size_ || begin >= end) {
            throw std::out_of_range("index out of range");
        }
        for (size_t i = begin; i < end; ++i) {
            data[i].~T();
        }
        for (size_t i = end; i < size_; ++i) {
            new (&data[i - (end - begin)]) T(std::move(data[i]));
            data[i].~T();
        }
        size_ -= (end - begin);
        return data + begin;
    }

    auto operator<=>(const VectorTheSerene &other) const {
        size_t min_size = std::min(size_, other.size_);
        for (size_t i = 0; i < min_size; ++i) {
            if (data[i] < other.data[i]) {
                return std::strong_ordering::less;
            } else if (data[i] > other.data[i]) {
                return std::strong_ordering::greater;
            }
        }
        return size_ <=> other.size_;
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
