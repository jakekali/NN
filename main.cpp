#include <iostream>
#include <vector>
#include "NN.cpp"
using json = nlohmann::json;



int fileEval(const std::string& filein, const std::string& fileout, NN &j){
    //open file
    std::ifstream in(filein);
    std::string currLine;

    //just to skip first line
    std::getline(in, currLine);
    auto sizes = split(currLine);
    while(std::getline(in, currLine)){
        std::getline(in, currLine);
        std::vector<double> ex = split(currLine);
        auto first = ex.cbegin();
        auto last = ex.cbegin() + sizes[1];
        auto first2 = ex.cbegin() + sizes[1];
        auto last2 = ex.cend();
        std::vector<double> inputVec(first, last);
        std::vector<double> outVec(first2,last2);

        auto ret = j.eval(inputVec);

        for(auto& it : ret){
            it = std::round(it);
        }
        std::cout <<  '\n';

    }
    return 0;

}
int main() {
    std::vector<int> jac = {30, 5, 1};
    NN j = NN(jac);
    j.loadWeightsFromFile("sInit.txt");
    fileEval("wdbc.test.txt", "d", j);
//    j.train("bctraining.txt",100,0.1);
//    j.exportNetwork("bec2.txt", true);
    return 0;
}