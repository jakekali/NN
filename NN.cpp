//
// Created by Jacob on 11/18/2021.
//

#include "NN.h"
#include <vector>
#include <iomanip>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

/*!
    @brief construct a new NN

    Returns a new NN, whose layers are of the sizes specified in the array.
    Inits Weights & Biases Randomly, First Node in everylayer (at index 0 is a fixed -1 input)
    Weights are stored going into any node

    @param std::vector<int> layersSizes {SizeOfInputLayer ...,Size of Hidden Layers, ... Size of Output Layer}

    @return a new NN, whose layers are of the sizes specified in the array.

    */
NN::NN(std::vector<int> layersSizesIn) {
    neuralNetwork.resize(layersSizesIn.size());
    for(int i = 0; i < layersSizesIn.size(); i++){
        layersSizes.push_back(layersSizesIn[i]);
        neuralNetwork[i].resize(layersSizesIn[i]+1);
        neuralNetwork[i][0].setCurrentValue(-1);
        neuralNetwork[i][0].fixed = true;
        neuralNetwork[i][0].weightsPrevious.resize(0);
        neuralNetwork[i][0].layer = i;
        neuralNetwork[i][0].index = 0;
        for(int j = 1; j < neuralNetwork[i].size();j++){
            neuralNetwork[i][j].fixed = false;
            neuralNetwork[i][j].setCurrentValue(0);
            if(i == 0){
                neuralNetwork[i][j].weightsPrevious.resize(0);
            }else{
                neuralNetwork[i][j].weightsPrevious.resize(neuralNetwork[i-1].size());
                for(double & weightsPreviou : neuralNetwork[i][j].weightsPrevious){
                    //random weights between -1 and 1
                    weightsPreviou = (double) 2.0 * rand() / RAND_MAX - 1.0;
                }
            }
        }
    }
}
/*!
    @brief updated all the incoming weights for a given node

    Returns -1 if the node does not exist else 0;

    @param int layer, int index, std::vector<double> weightsPrevious

    @return -1 if the node does not exist else 0;

    */
int NN::loadWeightsToNode(int layer, int index, std::vector<double> weightsPrevious) {
    if(neuralNetwork[layer-1].size() != weightsPrevious.size()){
        return -1;
    }
    for(int i = 0; i < neuralNetwork[layer][index].weightsPrevious.size(); i++){
        neuralNetwork[layer][index].weightsPrevious[i] = weightsPrevious[i];
    }
    return 0;
}

/*!
    @brief exports the NN into json or text file

    Returns if sable == true -> outputs a file with the fiven file name
            if sable != true -> returns a json object and writes file

    @param std::string filename, bool sable

    @return -if sable == true -> outputs a file with the fiven file name
            if sable != true -> returns a json object

    */

json NN::exportNetwork(const std::string& filename, bool sable) {
    //Sable - Standard Assigment format (.txt)
    if(sable){
        std::ofstream o(filename);
        for (int num : layersSizes){
            o << num << ' ';
        }
        o << '\n';
        for(int i = 1; i < layersSizes.size(); i ++){
            //this counter starts from one because the first fixed input node at index zero does not have any incoming weights
            for(int n = 1; n < neuralNetwork[i].size(); n++){
                for (double & weight : neuralNetwork[i][n].weightsPrevious){
                    o << weight << ' ';
                }
                o << '\n';
            }
        }
        o.close();
        return 0;
    }else{
    //!Sable - Json Format (.json)(!sable){
        json exportJ;
        exportJ["numLayers"] = layersSizes.size();
        exportJ["layersSize"] = layersSizes;
        for(int layerID = 0; layerID < layersSizes.size(); layerID++){
            for(int nodeID = 0; nodeID < neuralNetwork[layerID].size(); nodeID++){
                exportJ[std::to_string(layerID)][std::to_string(nodeID)]["layer"] = neuralNetwork[layerID][nodeID].layer;
                exportJ[std::to_string(layerID)][std::to_string(nodeID)]["index"] = neuralNetwork[layerID][nodeID].index;
                exportJ[std::to_string(layerID)][std::to_string(nodeID)]["fixed"] = neuralNetwork[layerID][nodeID].fixed;
                exportJ[std::to_string(layerID)][std::to_string(nodeID)]["currValue"] = neuralNetwork[layerID][nodeID].getCurrVal();
                for(int k = 0; k < neuralNetwork[layerID][nodeID].weightsPrevious.size(); k++) {
                    exportJ[std::to_string(layerID)][std::to_string(nodeID)]["weightsPrevious"][k] = neuralNetwork[layerID][nodeID].weightsPrevious[k];
                }
            }
        }

        std::ofstream o(filename+".json");
        o << std::setw(4) << exportJ << std::endl;
        return exportJ;
    }
}

/*!
    @brief sets the current value of a given node if the node is not fixed input

    Returns -1 if the node is a fixed input node.
            0 on success.

    @param int newValue

    @return -1 if the node is a fixed input node.
            0 on success.

    */
int NN::node::setCurrentValue(int newValue) {
    if(!fixed) {
        currValue = newValue;
    }else {
        std::cerr << "Bruh, you can't change fixed input";
        return -1;
    }
    return 0;
}
