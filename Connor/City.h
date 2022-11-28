#pragma once
#include <string>
//#include "OtherInfo.h"
using namespace std;


struct City {

    string name;
    int population;
    //string state;
    int size;
    double density;
    //int avgCostOfLiving;    //Annual income required to live in the city
    int avgHousingCost;
    int avgIncome;
    //int crimeRate;      //Number of crimes reported per 100000 people per year

    //Alternative city constructors
    //City(string name, int population, double density, int avgHousingCost, int size, int avgIncome);
    //City(string name, int population, string state, double size, double density, int avgCostOfLiving);
    //City(string name);

    City(const string &name, int population, int size, double density, int avgHousingCost, int avgIncome);

};


City::City(const string &name, int population, int size, double density, int avgHousingCost, int avgIncome) : name(
        name), population(population), size(size), density(density), avgHousingCost(avgHousingCost), avgIncome(
        avgIncome) {}
