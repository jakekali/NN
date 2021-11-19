#include <iostream>
#include <vector>
#include "NN.cpp"
#include "json.hpp"
using json = nlohmann::json;

int main() {
    std::vector<int> jac = {5,4,3};
    NN j = NN(jac);
    j.exportNetwork("dd.txt",true);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
