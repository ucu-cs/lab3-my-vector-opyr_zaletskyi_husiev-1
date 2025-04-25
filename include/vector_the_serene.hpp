#ifndef INCLUDE_VECTOR_THE_SERENE_HPP_
#define INCLUDE_VECTOR_THE_SERENE_HPP_

#include <cassert>
#include <compare>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T> class VectorTheSerene {
  private:
    size_t size_;
    size_t capacity_;
    // Assuming always valid:
    T *data;

    size_t capacity_for(size_t new_size) {
        size_t new_capacity = std::max(capacity_, 16ul);
        while (new_size > new_capacity) {
            new_capacity *= 2;
        }
        // Never auto-shrink
        return new_capacity;
    }

    static T *data_for(size_t new_capacity) {
        return static_cast<T *>(::operator new(sizeof(T) * new_capacity));
    }

    void move_into(T *new_data, size_t new_size, size_t new_capacity) {
        assert(new_capacity >= size_);
        try {
            for (size_t i = 0; i < size_; ++i) {
                new (&new_data[i]) T(std::move(data[i]));
            }
            auto old_data = data;
            auto old_size = size_;
            data = new_data;
            capacity_ = new_capacity;
            size_ = new_size;
            for (size_t i = 0; i < old_size; ++i) {
                old_data[i].~T();
            }
            ::operator delete(old_data);
        } catch (...) {
            // Clean up any constructed objects in new_data if an exception
            // occurs
            for (size_t i = 0; i < size_; ++i) {
                if (i < new_size && new_data != data) {
                    new_data[i].~T();
                }
            }
            ::operator delete(new_data);
            throw; // Re-throw the caught exception
        }
    }

    void unsafe_reserve(size_t new_capacity) {
        if (new_capacity == capacity_) {
            return;
        }

        T *new_data = data_for(new_capacity);
        move_into(new_data, size_, new_capacity);
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
        data = data_for(16);
    }
    VectorTheSerene(const VectorTheSerene &other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data = data_for(capacity_);
        try {
            for (size_t i = 0; i < size_; ++i) {
                // Can't just assign as this is the raw data
                new (&data[i]) T(other.data[i]);
            }
        } catch (...) {
            // Clean up any constructed objects if an exception occurs
            for (size_t i = 0; i < size_; ++i) {
                if (&data[i] != nullptr) {
                    data[i].~T();
                }
            }
            ::operator delete(data);
            throw; // Re-throw the caught exception
        }
    }
    VectorTheSerene(VectorTheSerene &&other) noexcept { swap(other); }
    VectorTheSerene &operator=(const VectorTheSerene &other) {
        VectorTheSerene tmp(other);
        swap(tmp);
        return *this;
    }

    VectorTheSerene(size_t n, const T &value) : size_(n), capacity_(0) {
        capacity_ = capacity_for(n);

        data = data_for(capacity_);
        try {
            for (size_t i = 0; i < size_; ++i) {
                new (&data[i]) T(value);
            }
        } catch (...) {
            // Clean up any constructed objects if an exception occurs
            for (size_t i = 0; i < size_; ++i) {
                if (&data[i] != nullptr) {
                    data[i].~T();
                }
            }
            ::operator delete(data);
            throw; // Re-throw the caught exception
        }
    }
    template <typename Iterator> VectorTheSerene(Iterator begin, Iterator end) {
        size_ = std::distance(begin, end);
        capacity_ = capacity_for(size_);
        data = data_for(capacity_);
        try {
            for (size_t i = 0; i < size_; ++i) {
                new (&data[i]) T(*(begin + i));
            }
        } catch (...) {
            // Clean up any constructed objects if an exception occurs
            for (size_t i = 0; i < size_; ++i) {
                if (&data[i] != nullptr) {
                    data[i].~T();
                }
            }
            ::operator delete(data);
            throw; // Re-throw the caught exception
        }
    }
    VectorTheSerene(std::initializer_list<T> list) {
        size_ = list.size();
        capacity_ = capacity_for(size_);
        data = data_for(capacity_);
        try {
            size_t i = 0;
            for (const auto &item : list) {
                new (&data[i++]) T(item);
            }
        } catch (...) {
            // Clean up any constructed objects if an exception occurs
            for (size_t i = 0; i < size_; ++i) {
                if (&data[i] != nullptr) {
                    data[i].~T();
                }
            }
            ::operator delete(data);
            throw; // Re-throw the caught exception
        }
    }

    VectorTheSerene &operator=(VectorTheSerene &&other) noexcept {
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
        auto new_capacity = capacity_for(size_ + 1);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            new (&target_data[size_])
                T(value); // First construct the new item - for cases like
                          // v.push_back(v.back())
            if (new_capacity != capacity_) {
                move_into(target_data, size_ + 1, new_capacity);
            } else {
                size_++;
            }
        } catch (...) {
            if (new_capacity != capacity_) {
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
        }
    }
    void push_back(T &&value) {
        auto new_capacity = capacity_for(size_ + 1);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            new (&target_data[size_]) T(std::move(value));
            if (new_capacity != capacity_) {
                move_into(target_data, size_ + 1, new_capacity);
            } else {
                size_++;
            }
        } catch (...) {
            if (new_capacity != capacity_) {
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
        }
    }

    void pop_back() {
        if (size_ > 0) {
            size_--;
            data[size_].~T();
        }
    }

    template <typename... Args> T &emplace_back(Args &&...args) {
        auto new_capacity = capacity_for(size_ + 1);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            new (&target_data[size_]) T(std::forward<Args>(args)...);
            if (new_capacity != capacity_) {
                move_into(target_data, size_ + 1, new_capacity);
            } else {
                size_++;
            }
            return target_data[size_ - 1];
        } catch (...) {
            if (new_capacity != capacity_) {
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
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
    size_t capacity() const { return capacity_; }

    bool is_empty() const { return size_ == 0; }
    bool empty() const { return size_ == 0; }
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data[i].~T();
        }
        size_ = 0;
        unsafe_reserve(capacity_for(0));
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            unsafe_reserve(new_capacity);
        }
    }
    void shrink_to_fit() {
        if (size_ < capacity_) {
            unsafe_reserve(size_);
        }
    }

    void resize(size_t new_size) {
        // Remove all the items >=new_size
        for (size_t i = new_size; i < size_; ++i) {
            data[i].~T();
        }
        unsafe_reserve(capacity_for(new_size));
        // Add new items
        try {
            for (size_t i = size_; i < new_size; ++i) {
                new (&data[i]) T();
            }
            size_ = new_size;
        } catch (...) {
            // If an exception occurs during construction, we need to clean up
            // and maintain a consistent state
            for (size_t i = size_; i < new_size; ++i) {
                if (&data[i] != nullptr) {
                    try {
                        data[i].~T();
                    } catch (...) {
                        // Ignore exceptions in destructors
                    }
                }
            }
            throw; // Re-throw the exception
        }
    }
    void resize(size_t new_size, const T &value) {
        // Remove all the items >=new_size
        for (size_t i = new_size; i < size_; ++i) {
            data[i].~T();
        }
        unsafe_reserve(capacity_for(new_size));
        // Add new items
        try {
            for (size_t i = size_; i < new_size; ++i) {
                new (&data[i]) T(value);
            }
            size_ = new_size;
        } catch (...) {
            // If an exception occurs during construction, we need to clean up
            for (size_t i = size_; i < new_size; ++i) {
                if (&data[i] != nullptr) {
                    try {
                        data[i].~T();
                    } catch (...) {
                        // Ignore exceptions in destructors
                    }
                }
            }
            throw; // Re-throw the exception
        }
    }

    iterator insert(const_iterator pos, const T &value) {
        size_t index = pos - data;
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }

        // TODO inserting part of self
        auto new_capacity = capacity_for(size_ + 1);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            for (size_t i = size_ - 1; i > index; --i) {
                new (&target_data[i]) T(std::move(data[i - 1]));
                data[i - 1].~T();
            }
            new (&target_data[index]) T(value);

            if (new_capacity != capacity_) {
                auto future_size = size_ + 1;
                size_ = index; // Since the rest was moved manually
                move_into(target_data, future_size, new_capacity);
                // If move_into fails, the vector has the shortened size, and of
                // course a memory leak happens, but it's still usable
            } else {
                size_++;
            }
        } catch (...) {
            // Clean up any allocated memory and constructed objects
            if (new_capacity != capacity_) {
                for (size_t i = 0; i < size_ && i != index; ++i) {
                    if (&target_data[i] != nullptr) {
                        try {
                            target_data[i].~T();
                        } catch (...) {
                            // Ignore exceptions in destructors
                        }
                    }
                }
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
        }

        return data + index;
    }

    iterator insert(const_iterator pos, T &&value) {
        size_t index = pos - data;
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }

        auto new_capacity = capacity_for(size_ + 1);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            for (size_t i = size_; i > index; --i) {
                new (&target_data[i]) T(std::move(data[i - 1]));
                data[i - 1].~T();
            }
            new (&target_data[index]) T(std::move(value));

            if (new_capacity != capacity_) {
                auto future_size = size_ + 1;
                size_ = index; // Since the rest was moved manually
                move_into(target_data, future_size, new_capacity);
            } else {
                size_++;
            }
        } catch (...) {
            // Clean up any allocated memory and constructed objects
            if (new_capacity != capacity_) {
                for (size_t i = 0; i < size_ && i != index; ++i) {
                    if (&target_data[i] != nullptr) {
                        try {
                            target_data[i].~T();
                        } catch (...) {
                            // Ignore exceptions in destructors
                        }
                    }
                }
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
        }

        return data + index;
    }

    template <typename Iterator>
    iterator insert(const_iterator pos, Iterator begin, Iterator end) {
        size_t index = pos - data;
        if (index > size_) {
            throw std::out_of_range("index out of range");
        }
        size_t count = std::distance(begin, end);

        if (count == 0) {
            return data + index;
        }

        auto new_capacity = capacity_for(size_ + count);
        auto target_data =
            new_capacity == capacity_ ? data : data_for(new_capacity);
        try {
            if (size_ > 0) {
                for (size_t i = size_ - 1; i >= index; --i) {
                    new (&target_data[i + count]) T(std::move(data[i]));
                    data[i].~T();
                }
            }
            for (size_t i = 0; i < count; ++i) {
                new (&target_data[index + i]) T(*(begin + i));
            }

            if (new_capacity != capacity_) {
                auto future_size = size_ + count;
                size_ = index; // Since the rest was moved manually
                move_into(target_data, future_size, new_capacity);
            } else {
                size_ += count;
            }
        } catch (...) {
            // Clean up any allocated memory and constructed objects
            if (new_capacity != capacity_) {
                for (size_t i = 0; i < size_ && i != index; ++i) {
                    if (&target_data[i] != nullptr) {
                        try {
                            target_data[i].~T();
                        } catch (...) {
                            // Ignore exceptions in destructors
                        }
                    }
                }
                ::operator delete(target_data);
            }
            throw; // Re-throw the caught exception
        }

        return data + index;
    }

    iterator erase(const_iterator pos) {
        size_t index = pos - data;
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

    iterator erase(const_iterator begin, const_iterator end) {
        size_t first = begin - data;
        size_t last = end - data;

        if (first >= size_ || last > size_ || first >= last || begin < data ||
            end < data) {
            throw std::out_of_range("index out of range");
        }

        for (size_t i = first; i < last; ++i) {
            data[i].~T();
        }
        size_t count = last - first;
        for (size_t i = last; i < size_; ++i) {
            new (&data[i - count]) T(std::move(data[i]));
            data[i].~T();
        }
        size_ -= count;
        return data + first;
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
