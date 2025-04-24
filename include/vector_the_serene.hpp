#ifndef INCLUDE_VECTOR_THE_SERENE_HPP_
#define INCLUDE_VECTOR_THE_SERENE_HPP_

#include <cstddef>
#include <iostream>
#include <utility>

template <typename T> class VectorTheSerene {
  private:
    size_t size_;
    size_t capacity;
    T *data;
    bool allocate_more() {
        if (size_ <= 8) {
            if (capacity == 16) {
                return false;
            }
            capacity = 16;
        } else if (size_ >= capacity / 2) {
            capacity = 2 * capacity;
        } else {
            return false;
        }
        allocate();
        return true;
    }
    bool allocate_less() {
        if (size_ <= 8) {
            if (capacity == 16) {
                return false;
            }
            capacity = 16;
        } else if (size_ <= capacity / 4) {
            capacity = capacity / 2;
        } else {
            return false;
        }
        allocate();
        return true;
    }
    void allocate() {
        T *new_data = new T[capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
    }

  public:
    void swap(VectorTheSerene &other) {
        std::swap(size_, other.size_);
        std::swap(capacity, other.capacity);
        std::swap(data, other.data);
    }

    VectorTheSerene() {
        size_ = 0;
        capacity = 8;
        data = new T[capacity];
    }
    VectorTheSerene(const VectorTheSerene &other) {
        size_ = other.size_;
        capacity = other.capacity;
        data = new T[capacity];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
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
        data = new T[capacity];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = value;
        }
    }
    template <typename Iterator>
    VectorTheSerene(Iterator begin, Iterator end) {
        size_ = std::distance(begin, end);
        if (size_ <= 8) {
            capacity = 16;
        } else {
            capacity = size_;
        }
        data = new T[capacity];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = *(begin + i);
        }
    }
    VectorTheSerene (std::initializer_list<T> list) {
        size_ = list.size();
        if (size_ <= 8) {
            capacity = 16;
        } else {
            capacity = size_;
        }
        data = new T[capacity];
        size_t i = 0;
        for (const auto &item : list) {
            data[i++] = item;
        }
    }


    VectorTheSerene &operator=(VectorTheSerene &&other) {
        swap(other);
        return *this;
    }
    ~VectorTheSerene() { delete[] data; }

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
        size_++;
        allocate_more();
        data[size_ - 1] = value;
    }
    void push_back(T &&value) {
        size_++;
        allocate_more();
        data[size_ - 1] = std::move(value);
    }

    void pop_back() {
        if (size_ > 0) {
            size_--;
            allocate_less();
        }
    }

    size_t size() const { return size_; }
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
