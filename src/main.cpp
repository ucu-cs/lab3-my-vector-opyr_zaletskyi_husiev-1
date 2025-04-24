#include "./vector_the_serene.hpp"
#include <vector>

// To check for extra operations
class ConstructReporter {
  public:
    ConstructReporter() { std::cout << "Constructed\n"; }
    ConstructReporter(const ConstructReporter &) {
        std::cout << "Copy constructed\n";
    }
    ConstructReporter(ConstructReporter &&) noexcept {
        std::cout << "Move constructed\n";
    }
    ConstructReporter &operator=(const ConstructReporter &) {
        std::cout << "Copy assigned\n";
        return *this;
    }
    ConstructReporter &operator=(ConstructReporter &&) noexcept {
        std::cout << "Move assigned\n";
        return *this;
    }
    ~ConstructReporter() { std::cout << "Destructed\n"; }
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

    VectorTheSerene<ConstructReporter> v4;
    v4.push_back(ConstructReporter());
    v4.push_back(ConstructReporter());

    return 0;
}
