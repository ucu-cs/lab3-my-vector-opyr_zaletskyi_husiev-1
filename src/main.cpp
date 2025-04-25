#include "../include/array_the_steadfast.hpp"
#include "./vector_the_serene.hpp"
#include <algorithm>
#include <iostream>
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
};

int main() {
    VectorTheSerene<int> v;
    v.push_back(1);
    v.push_back(2);
    print_vector(v);

    VectorTheSerene<int> v2 = {1, 2, 3, 4};
    std::vector<int> std_v = {3};
    std::copy(std_v.begin(), std_v.end(), std::back_inserter(v2));
    print_vector(v2);
    v2.clear();
    print_vector(v2);
    v2.push_back(1);
    v2.push_back(2);
    print_vector(v2);

    VectorTheSerene<std::string> v3;
    v3.push_back("Hello");
    v3.push_back("World");
    print_vector(v3);

    {
        VectorTheSerene<ConstructReporter> v4;
        for (int i = 0; i < 50; ++i) {
            v4.emplace_back(std::to_string(i));
        }
    }

    VectorTheSerene<std::string> v5;
    for (int i = 0; i < 50; ++i) {
        v5.insert(v5.begin(), std::to_string(i));
    }
    print_vector(v5);

    ArrayTheSteadfast<int, 5> arr = {1, 2, 3, 4, 5};
    print_array(arr);

    return 0;
}
