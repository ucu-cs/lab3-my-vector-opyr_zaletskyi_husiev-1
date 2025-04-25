#include "./array_the_steadfast.hpp"
#include "./vector_the_serene.hpp"
#include <algorithm>
#include <compare>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// To check for extra operations
class ConstructReporter {
  private:
    std::string id;

  public:
    ConstructReporter() {
        id = "default";
        std::cout << "Default constructed" << std::endl;
    }
    ConstructReporter(std::string id) : id(id) {
        std::cout << "Constructed " << id << std::endl;
    }
    ConstructReporter(const ConstructReporter &other) {
        std::cout << "Copy constructed from " << other.id << std::endl;
        id = "Copy of " + other.id;
    }
    ConstructReporter(ConstructReporter &&other) noexcept {
        std::cout << "Move constructed from " << other.id << std::endl;
        id = "Move of " + other.id;
    }
    ConstructReporter &operator=(const ConstructReporter &other) {
        std::cout << "Copy assigned " << other.id << " into what was " << id
                  << std::endl;
        id = "Copy of " + other.id;
        return *this;
    }
    ConstructReporter &operator=(ConstructReporter &&other) noexcept {
        std::cout << "Move assigned " << other.id << " into what was " << id
                  << std::endl;
        id = "Move of " + other.id;
        return *this;
    }
    ~ConstructReporter() { std::cout << "Destructed " << id << std::endl; }

    friend std::ostream &operator<<(std::ostream &os,
                                    const ConstructReporter &cr) {
        return os << cr.id;
    }

    bool operator==(const ConstructReporter &other) const {
        return id == other.id;
    }

    auto operator<=>(const ConstructReporter &other) const {
        return id <=> other.id;
    }
};

void test_vector_functionality() {
    std::cout << "\n=== VectorTheSerene Basic Operations ===\n";
    VectorTheSerene<int> v;
    v.push_back(1);
    v.push_back(2);
    std::cout << "After push_back: ";
    print_vector(v);

    VectorTheSerene<int> v2 = {1, 2, 3, 4};
    std::cout << "Initializer list: ";
    print_vector(v2);

    std::vector<int> std_v = {5, 6, 7};
    std::cout << "Before copy: ";
    print_vector(v2);
    std::copy(std_v.begin(), std_v.end(), std::back_inserter(v2));
    std::cout << "After copy with back_inserter: ";
    print_vector(v2);

    v2.clear();
    std::cout << "After clear - size: " << v2.size()
              << ", empty: " << (v2.empty() ? "true" : "false") << std::endl;

    v2.resize(5, 10);
    std::cout << "After resize(5, 10): ";
    print_vector(v2);

    std::cout << "Front: " << v2.front() << ", Back: " << v2.back()
              << std::endl;

    try {
        std::cout << "Element at index 2: " << v2.at(2) << std::endl;
        std::cout << "Element at index 10: " << v2.at(10) << std::endl;
    } catch (const std::out_of_range &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    v2.insert(v2.begin() + 2, 99);
    std::cout << "After insert at position 2: ";
    print_vector(v2);

    v2.erase(v2.begin() + 1);
    std::cout << "After erase at position 1: ";
    print_vector(v2);

    v2.erase(v2.begin(), v2.begin() + 2);
    std::cout << "After erase range [0,2): ";
    print_vector(v2);

    std::cout << "Capacity before reserve: " << v2.capacity() << std::endl;
    v2.reserve(100);
    std::cout << "Capacity after reserve(100): " << v2.capacity() << std::endl;

    v2.shrink_to_fit();
    std::cout << "Capacity after shrink_to_fit: " << v2.capacity() << std::endl;

    std::cout << "\n=== VectorTheSerene with Strings ===\n";
    VectorTheSerene<std::string> v3;
    v3.push_back("Hello");
    v3.push_back("World");
    print_vector(v3);

    std::cout << "\n=== Testing Vector Comparison Operators ===\n";
    {
        VectorTheSerene<int> a;
        a.push_back(1);
        a.push_back(2);
        a.push_back(3);

        VectorTheSerene<int> b;
        b.push_back(1);
        b.push_back(2);
        b.push_back(3);

        VectorTheSerene<int> c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(4);

        std::cout << "Direct comparison using operators:\n";

        auto result = a <=> b;
        std::cout << "Result of a <=> b: "
                  << (result == std::strong_ordering::equal
                          ? "equal"
                          : (result == std::strong_ordering::less ? "less"
                                                                  : "greater"))
                  << std::endl;

        auto result2 = a <=> c;
        std::cout << "Result of a <=> c: "
                  << (result2 == std::strong_ordering::equal
                          ? "equal"
                          : (result2 == std::strong_ordering::less ? "less"
                                                                   : "greater"))
                  << std::endl;
    }

    std::cout << "\n=== Vector of Vectors ===\n";
    {
        VectorTheSerene<VectorTheSerene<int>> vv;

        VectorTheSerene<int> inner1;
        inner1.push_back(1);
        inner1.push_back(2);
        inner1.push_back(3);
        vv.push_back(inner1);

        VectorTheSerene<int> inner2;
        inner2.push_back(4);
        inner2.push_back(5);
        inner2.push_back(6);
        vv.push_back(inner2);

        std::cout << "Vector of vectors contents:\n";
        for (size_t i = 0; i < vv.size(); ++i) {
            std::cout << "Inner vector " << i << ": ";
            for (size_t j = 0; j < vv[i].size(); ++j) {
                std::cout << vv[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "\n=== Using with STL Algorithms ===\n";
    VectorTheSerene<int> v4;
    v4.resize(10, 0);
    std::iota(v4.begin(), v4.end(), 1);
    std::cout << "After iota: ";
    print_vector(v4);

    std::transform(v4.begin(), v4.end(), v4.begin(),
                   [](int x) { return x * 2; });
    std::cout << "After transform (x2): ";
    print_vector(v4);

    std::cout << "Using for_each: ";
    std::for_each(v4.begin(), v4.end(), [](int x) { std::cout << x << " "; });
    std::cout << std::endl;
}

void test_array_functionality() {
    std::cout << "\n=== ArrayTheSteadfast Basic Operations ===\n";
    ArrayTheSteadfast<int, 5> arr = {1, 2, 3, 4, 5};
    std::cout << "Array after initialization: ";
    print_array(arr);

    ArrayTheSteadfast<int, 3> arr2(42);
    std::cout << "Array with all elements 42: ";
    print_array(arr2);

    try {
        std::cout << "Element at index 2: " << arr.at(2) << std::endl;
        std::cout << "Element at index 10: " << arr.at(10) << std::endl;
    } catch (const std::out_of_range &e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }

    std::cout << "Front: " << arr.front() << ", Back: " << arr.back()
              << std::endl;

    ArrayTheSteadfast<int, 5> arr3 = {10, 20, 30, 40, 50};
    std::cout << "Before swap - arr: ";
    print_array(arr);
    std::cout << "Before swap - arr3: ";
    print_array(arr3);

    arr.swap(arr3);
    std::cout << "After swap - arr: ";
    print_array(arr);
    std::cout << "After swap - arr3: ";
    print_array(arr3);

    std::cout << "\n=== Testing Comparison Operators (Array) ===\n";
    ArrayTheSteadfast<int, 3> a = {1, 2, 3};
    ArrayTheSteadfast<int, 3> b = {1, 2, 3};
    ArrayTheSteadfast<int, 3> c = {1, 2, 4};

    std::cout << "a == b: " << (a == b ? "true" : "false") << std::endl;
    std::cout << "a != c: " << (a != c ? "true" : "false") << std::endl;

    auto result = a <=> c;
    std::cout << "Result of a <=> c: "
              << (result == std::strong_ordering::equal
                      ? "equal"
                      : (result == std::strong_ordering::less ? "less"
                                                              : "greater"))
              << std::endl;

    std::cout << "\n=== Using Array with STL Algorithms ===\n";
    ArrayTheSteadfast<int, 5> arr4 = {0, 0, 0, 0, 0};
    std::iota(arr4.begin(), arr4.end(), 1);
    std::cout << "After iota: ";
    print_array(arr4);

    std::transform(arr4.begin(), arr4.end(), arr4.begin(),
                   [](int x) { return x * 3; });
    std::cout << "After transform (x3): ";
    print_array(arr4);

    std::cout << "Using for_each: ";
    std::for_each(arr4.begin(), arr4.end(),
                  [](int x) { std::cout << x << " "; });
    std::cout << std::endl;

    std::reverse(arr4.begin(), arr4.end());
    std::cout << "After reverse: ";
    print_array(arr4);

    std::cout << "\n=== Nested Arrays ===\n";
    ArrayTheSteadfast<ArrayTheSteadfast<int, 2>, 2> nested = {{1, 2}, {3, 4}};
    for (size_t i = 0; i < nested.size(); ++i) {
        for (size_t j = 0; j < nested[i].size(); ++j) {
            std::cout << nested[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n=== Testing with Complex Classes ===\n";
    VectorTheSerene<ConstructReporter> v_cr;
    std::cout << "Pushing elements to VectorTheSerene<ConstructReporter>:"
              << std::endl;
    v_cr.push_back(ConstructReporter("First"));
    v_cr.push_back(ConstructReporter("Second"));
    std::cout << "Elements in vector: " << v_cr.size() << std::endl;

    ArrayTheSteadfast<ConstructReporter, 2> arr_cr;
    std::cout << "Assigning to ArrayTheSteadfast<ConstructReporter, 2>:"
              << std::endl;
    arr_cr[0] = ConstructReporter("Array First");
    arr_cr[1] = ConstructReporter("Array Second");
}

int main() {
    test_vector_functionality();
    test_array_functionality();

    std::cout << "\n=== Nested Containers Tests ===\n";
    VectorTheSerene<ArrayTheSteadfast<int, 3>> v_of_a;
    v_of_a.push_back(ArrayTheSteadfast<int, 3>{1, 2, 3});
    v_of_a.push_back(ArrayTheSteadfast<int, 3>{4, 5, 6});

    std::cout << "VectorTheSerene of ArrayTheSteadfast contents:\n";
    for (size_t i = 0; i < v_of_a.size(); ++i) {
        std::cout << "Array " << i << ": ";
        for (size_t j = 0; j < v_of_a[i].size(); ++j) {
            std::cout << v_of_a[i][j] << " ";
        }
        std::cout << std::endl;
    }

    ArrayTheSteadfast<VectorTheSerene<int>, 2> a_of_v;
    a_of_v[0].push_back(10);
    a_of_v[0].push_back(20);
    a_of_v[1].push_back(30);
    a_of_v[1].push_back(40);

    std::cout << "ArrayTheSteadfast of VectorTheSerene contents:\n";
    for (size_t i = 0; i < a_of_v.size(); ++i) {
        std::cout << "Vector " << i << ": ";
        for (size_t j = 0; j < a_of_v[i].size(); ++j) {
            std::cout << a_of_v[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n=== Testing ConstructReporter with VectorTheSerene ===\n";
    {
        std::cout << "Creating vector with ConstructReporter objects...\n";
        VectorTheSerene<ConstructReporter> v4;
        for (int i = 0; i < 5; ++i) {
            v4.emplace_back(std::to_string(i));
        }

        std::cout << "\nInserting at beginning...\n";
        v4.insert(v4.begin(), ConstructReporter("inserted-at-begin"));

        std::cout << "\nErasing from middle...\n";
        v4.erase(v4.begin() + 2);

        std::cout << "\nResizing to smaller size...\n";
        v4.resize(3);

        std::cout << "\nResizing to larger size with default value...\n";
        v4.resize(5);

        std::cout << "\nVector going out of scope...\n";
    }

    return 0;
}
