#include "./vector_the_serene.hpp"
#include <vector>

int main() {
    VectorTheSerene<int> v;
    v.push_back(1);
    v.push_back(2);
    print_vector(v);
    VectorTheSerene<int> v2 = {1,2,3,4};
    std::vector<int> std_v = {3};
    std::copy(std_v.begin(), std_v.end(), std::back_inserter(v2));
    print_vector(v2);
    v2.clear();
    print_vector(v2);
    v2.push_back(1);
    v2.push_back(2);
    print_vector(v2);
    return 0;
}
