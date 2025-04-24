#include "./vector_the_serene.hpp"
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

    for (int i = 0; i < 50; ++i) {
        v4.push_back(ConstructReporter(std::to_string(i)));
    }

    return 0;
}
