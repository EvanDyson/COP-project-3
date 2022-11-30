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