#include "./vector_the_serene.hpp"
#include <vector>

int main() {
    VectorTheSerene<int> v;
    v.push_back(1);
    v.push_back(2);
    print_vector(v);
    return 0;
}
