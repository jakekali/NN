#include <iostream>
#include <vector>
#include "NN.cpp"
#include "stat.cpp"
using json = nlohmann::json;



int fileEval(const std::string& filein, const std::string& fileout, NN &j){
    //open file
    std::ifstream in(filein);
    std::string currLine;

    std::filebuf fb;
    fb.open (fileout,std::ios::out);
    std::ostream os(&fb);

    //just to skip first line
    std::getline(in, currLine);
    std::vector<stat> stats;
    auto sizes = split(currLine);
    stats.resize(sizes[2]);
    int k = 0;
    while(std::getline(in, currLine) && k < sizes[0]){
        std::vector<double> ex = split(currLine);
        auto first = ex.cbegin();
        auto last = ex.cbegin() + sizes[1];
        auto first2 = ex.cbegin() + sizes[1];
        auto last2 = ex.cend();
        std::vector<double> inputVec(first, last);
        std::vector<double> outVec(first2,last2);

        auto ret = j.eval(inputVec);

        for(int i = 0; i < stats.size(); i++){
            //ret[i] = std::round(ret[i]);

            if(ret[i] > 0.5 && outVec[i] == 1){
                stats[i].A++;
            }else if(ret[i] > 0.5 && outVec[i] == 0){
                stats[i].B++;
            }else if(ret[i] <= 0.5 && outVec[i] == 1){
                stats[i].C++;
            }else if(ret[i] <= 0.5 && outVec[i] == 0){
                stats[i].D++;
            }
        }
        k++;
    }
    stat totStats;
    double acc;
    double prec;
    double recall;


    for(int i = 0; i < stats.size(); i++) {
        //Micro
        totStats.A += stats[i].A;
        totStats.B += stats[i].B;
        totStats.C += stats[i].C;
        totStats.D += stats[i].D;
        //Macro
        acc += stats[i].getAccuracy();
        prec += stats[i].getPrecision();
        recall += stats[i].getRecall();

        os << stats[i].A << " " << stats[i].B << " " << stats[i].C << " " << stats[i].D << " ";
        os <<  std::setprecision(3) << std::fixed << stats[i].getAccuracy() << " ";
        os <<  std::setprecision(3) << std::fixed << stats[i].getPrecision() << " ";
        os <<  std::setprecision(3) << std::fixed << stats[i].getRecall() << " ";
        os <<  std::setprecision(3) << std::fixed << stats[i].getF1() << "\n";
    }

    //Get Micro Average

    os <<  std::setprecision(3) << std::fixed << totStats.getAccuracy() << " ";
    os <<  std::setprecision(3) << std::fixed << totStats.getPrecision() << " ";
    os <<  std::setprecision(3) << std::fixed << totStats.getRecall() << " ";
    os <<  std::setprecision(3) << std::fixed << totStats.getF1() << "\n";

    //Get Macro Averages
    os <<  std::setprecision(3) << std::fixed << acc/stats.size() << " ";
    os <<  std::setprecision(3) << std::fixed << prec/stats.size() << " ";
    os <<  std::setprecision(3) << std::fixed << recall/stats.size() << " ";
    os <<  std::setprecision(3) << std::fixed << (2 * prec/stats.size() * recall/stats.size())/(prec/stats.size() + recall/stats.size()) << "\n";


    fb.close();

    return 0;

}
int main() {
    std::vector<int> jac = {30, 5, 1};
    NN j = NN(jac);
    j.loadWeightsFromFile("wdbc.trained.txt");
    fileEval("wdbc.test.txt", "df.txt", j);
//    j.train("bctraining.txt",100,0.1);
//    j.exportNetwork("bec2.txt", true);
    return 0;
}