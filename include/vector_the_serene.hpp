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
using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<T*>;
    using const_reverse_iterator = std::reverse_iterator<const T*>;
    
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
    template <typename Iterator> VectorTheSerene(Iterator begin, Iterator end) {
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
    VectorTheSerene(std::initializer_list<T> list) {
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

    T* begin() { return data; }
    T* end() { return data + size_; }
    
    const T* begin() const { return data; }
    const T* end() const { return data + size_; }
    
    const T* cbegin() const { return data; }
    const T* cend() const { return data + size_; }
    
    std::reverse_iterator<T*> rbegin() { return std::reverse_iterator<T*>(end()); }
    std::reverse_iterator<T*> rend() { return std::reverse_iterator<T*>(begin()); }
    
    std::reverse_iterator<const T*> rbegin() const { return std::reverse_iterator<const T*>(end()); }
    std::reverse_iterator<const T*> rend() const { return std::reverse_iterator<const T*>(begin()); }
    
    std::reverse_iterator<const T*> crbegin() const { return std::reverse_iterator<const T*>(cend()); }
    std::reverse_iterator<const T*> crend() const { return std::reverse_iterator<const T*>(cbegin()); }
    
    size_t size() const { return size_; }

    bool is_empty() const { return size_ == 0; }
    void clear() {
        size_ = 0;
        allocate_less();
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
