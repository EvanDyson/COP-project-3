#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class City {
    string name;
    int popSize, popDensity, avgHousingCost, citySize, avgIncome;
    int weightScore = 0;

public:
    City(string _name, int _popSize, int _popDensity, int _avgHousingCost, int _citySize, int _avgIncome) : name(_name),
        popSize(_popSize), popDensity(_popDensity), avgHousingCost(_avgHousingCost), citySize(_citySize), avgIncome(_avgIncome) {};
    string getName();
    int getPopSize() const;
    int getPopDensity() const;
    int getAvgHousingCost() const;
    int getCitySize() const;
    int getAvgIncome() const;
    int getWeightScore() const;
    void setWeightScore(double rank);
};