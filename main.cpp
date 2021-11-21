#include <iostream>
#include <vector>
#include "NN.cpp"
using json = nlohmann::json;

int main() {
    std::vector<int> jac = {30,5,1};
    NN j = NN(jac);
    j.loadWeightsFromFile("sInit.txt");
    auto b = j.eval({0.656,0.264,0.657,0.400,0.831,0.804,0.704,0.769,0.796,0.808,0.430,0.185,0.461,0.283,0.206,0.362,0.136,0.301,0.380,0.208,0.704,0.350,0.735,0.475,0.729,0.710,0.741,0.912,0.693,0.687});
    auto c = j.deltas({0});
    auto d = j.updateWeights(0.1);
    j.exportNetwork("tdd.txt", true);
    return 0;
}
