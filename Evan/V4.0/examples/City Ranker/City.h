#pragma once
//#include <string>
//#include "OtherInfo.h"
using namespace std;


struct City {

    string name;
    int population;
    int size;
    double density;
    int avgHousingCost;
    int avgIncome;
    int weightScore;

    // default constructor
    City() : name("DEFAULT"), population(0), size(0), density(0), avgHousingCost(0), avgIncome(0), weightScore(0) {}
    City(string name, int population, int size, double density, int avgHousingCost, int avgIncome) : name(
        name), population(population), size(size), density(density), avgHousingCost(avgHousingCost), avgIncome(
            avgIncome)
    {
        // SET WEIGHTS IN HERE
        weightScore = population / 2;
    }
    void operator=(const City& c);
    void setWeight();
    void setWeight(int population, int size, float density, int avgHousingCost, int avgIncome);
    void setWeightScore(int x) { weightScore = x; }
    int getWeightScore() { return weightScore; }
};

void City::operator=(const City& c) 
{
    name = c.name;
    population = c.population;
    size = c.size;
    density = c.density;
    avgHousingCost = c.avgHousingCost;
    avgIncome = c.avgIncome;
    weightScore = c.weightScore;
}

void City::setWeight()
{
    weightScore = population / 2;
}
void City::setWeight(int population_, int size_, float density_, int avgHousingCost_, int avgIncome_)
{
    /*      ImGui::SliderInt("Population", &i1, 500, 4800000);
            ImGui::SliderInt("City Size", &i2, 10, 1400);
            ImGui::SliderFloat("Population density", &i3, 0.48f, 400000);
            ImGui::SliderInt("Avg. income", &i4, 10000, 950000);
            ImGui::SliderInt("Avg. housing costs", &i5, 10000, 2400000);*/
    float cPop = 0, cSize = 0, cDens = 0, cHouse = 0, cInc = 0, cScore = 0, numInputs = 0;
    
    if (population_ != 0)
        cPop = abs(((float)population - population_) / 4800000); numInputs++;
    if (size_ != 0)
        cSize = abs(((float)size - size_) / 1400); numInputs++;
    if (density_ != 0)
        cDens = abs(((float)density - density_) / 400000); numInputs++;
    if (avgHousingCost_ != 0)
        cHouse = abs(((float)avgHousingCost - avgHousingCost_) / 2400000); numInputs++;
    if (avgIncome_ != 0)
        cInc = abs(((float)avgIncome - avgIncome_) / 950000); numInputs++;
    //cout << cPop << " " << cSize << " " << cDens << " " << cHouse << " " << cInc << endl;
    cScore = (((cPop + cSize + cDens + cHouse + cInc) / numInputs) * 100);
    weightScore = cScore;
}
/*
    score = [ | (population - user input) / max population | + 
            | (size - user input) / max size | + 
            | (density - user input) / max density | +
            | (avg. housing cost - user input) / max avg. housing cost | + 
            | (avg. income - user input) / max avg. income | ]
            / # of inputs used (max of 5)
    
            * ALL ABOVE * x100
            

     Values based off of Random Skewed csv file
   |----------------------------------------------|
   |     Catagory     |   HIGHEST   |    LOWEST   |
   | population       |  4,814,431  |     500     |
   | size             |   404,726   |    0.478    |
   | density          |     1410    |      10     |
   | avg housing cost |   954,678   |    10,000   |
   | avg income       |  2,448,463  |    10,000   |
   |----------------------------------------------|
*/
