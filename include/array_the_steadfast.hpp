#ifndef INCLUDE_ARRAY_THE_STEADFAST_HPP_
#define INCLUDE_ARRAY_THE_STEADFAST_HPP_

#include <compare>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <utility>

template <typename T, size_t N> class ArrayTheSteadfast {
  private:
    T data_[N];

  public:
    using value_type = T;
    using iterator = T *;
    using const_iterator = const T *;
    using reverse_iterator = std::reverse_iterator<T *>;
    using const_reverse_iterator = std::reverse_iterator<const T *>;

    ArrayTheSteadfast() = default;

    ArrayTheSteadfast(const T &value) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = value;
        }
    }

    ArrayTheSteadfast(std::initializer_list<T> list) {
        auto it = list.begin();
        size_t i = 0;

        for (; i < N && it != list.end(); ++i, ++it) {
            data_[i] = *it;
        }

        for (; i < N; ++i) {
            data_[i] = T();
        }
    }

    T &operator[](size_t index) { return data_[index]; }
    const T &operator[](size_t index) const { return data_[index]; }

    T &at(size_t index) {
        if (index >= N) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    const T &at(size_t index) const {
        if (index >= N) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    T &front() { return data_[0]; }
    const T &front() const { return data_[0]; }

    T &back() { return data_[N - 1]; }
    const T &back() const { return data_[N - 1]; }

    T *data() { return data_; }
    const T *data() const { return data_; }

    iterator begin() { return data_; }
    iterator end() { return data_ + N; }

    const_iterator begin() const { return data_; }
    const_iterator end() const { return data_ + N; }

    const_iterator cbegin() const { return data_; }
    const_iterator cend() const { return data_ + N; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    constexpr size_t size() const { return N; }
    constexpr bool is_empty() const { return N == 0; }

    void swap(ArrayTheSteadfast &other) {
        for (size_t i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

    bool operator==(const ArrayTheSteadfast &other) const {
        for (size_t i = 0; i < N; ++i) {
            if (!(data_[i] == other.data_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const ArrayTheSteadfast &other) const {
        return !(*this == other);
    }

    auto operator<=>(const ArrayTheSteadfast &other) const {
        for (size_t i = 0; i < N; ++i) {
            if (data_[i] < other.data_[i]) {
                return std::strong_ordering::less;
            } else if (data_[i] > other.data_[i]) {
                return std::strong_ordering::greater;
            }
        }
        return std::strong_ordering::equal;
    }
};

template <typename T, size_t N>
void print_array(const ArrayTheSteadfast<T, N> &arr) {
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T, size_t N> using my_array = ArrayTheSteadfast<T, N>;

#endif // INCLUDE_ARRAY_THE_STEADFAST_HPP_
