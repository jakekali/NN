//
// Created by Jacob on 11/18/2021.
//
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

#ifndef NN_NN_H
#define NN_NN_H


class NN {
public:
    NN(std::vector<int> layersSizesIn);
    int loadWeightsToNode (int layer, int index, std::vector<double> weightsPrevious);
    json exportNetwork(const std::string& filename, bool sable);
private:
    class node{
        int currValue;
    public:
        int index;
        int layer;
        int setCurrentValue(int newValue);
        std::vector<double> weightsPrevious;
        bool fixed = false;
        int getCurrVal(){return currValue;};
    };
    std::vector<int> layersSizes;
    std::vector<std::vector<node>> neuralNetwork;
};


#endif //NN_NN_H
