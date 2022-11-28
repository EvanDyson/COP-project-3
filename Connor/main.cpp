#include <iostream>
#include <fstream>
#include "Generator.h"
#include <map>

using namespace std;

void writeFile(vector<City> &c) {

    ofstream file;
    file.open("cities.csv");

    //file << "City Name,State,Population,Size,Density,Cost of Living\n";
    /*for (int i = 0; i < c.size(); i++) {
    file << c[i].name << "," << c[i].state << "," << c[i].population << ","
    << c[i].size << "," << c[i].density << "," << c[i].avgCostOfLiving << "\n";
    }*/

    file << "Name,Population,Pop. Density,Avg. Housing Cost,Size,Avg. Income\n";
    for (int i = 0; i < c.size(); i++) {
        file << c[i].name << "," << c[i].population << "," << c[i].density << ","
             << c[i].avgHousingCost << "," << c[i].size << "," << c[i].avgIncome << "\n";
    }

}

int main() {

    Generator g;

    std::map<int, int> hist{};
    /*for(int n=0; n<10000; ++n) {
        //int output = (int) g.clip(g.gaussianRandom(50, 100) + 200, 50, 1000);
        //int output = g.clip(pow(g.gaussianRandom(50, 5), 0.8), 0, 500);
        //int output = 5 + sqrt(g.Random(30, 1000));
        // output = g.Random(5, g.Random(6, g.Random(7, g.Random(8, g.Random(9, 5000)))));
        int output = pow(g.Random(1, 100), 0.65) / 2;//sqrt(g.Random(1, 50));
        ++hist[output];
    }
    for(auto p : hist) {
        std::cout << std::setw(2)
                  << p.first << ' ' << std::string(p.second/50, '*') << '\n';
    }*/


    //City c[100];
    //g.generateCities(, 100);

    //for (int i = 0; i < 20; i++) {
    //    cout << g.generateCityName() << endl;
    //}

    vector<City> c;
    g.generateCities(c, 100000);
    writeFile(c);

    return 0;
}
