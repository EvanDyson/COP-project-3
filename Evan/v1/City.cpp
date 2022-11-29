#include "City.h"

std::string City::getName()
{
    return name;
}

int City::getPopSize() const
{
    return popSize;
}

int City::getPopDensity() const
{
    return popDensity;
}

int City::getAvgHousingCost() const
{
    return avgHousingCost;
}

int City::getCitySize() const
{
    return citySize;
}

int City::getAvgIncome() const
{
    return avgIncome;
}

int City::getWeightScore() const
{
    return weightScore;
}

void City::setWeightScore(double rank)
{
    weightScore = rank;
}