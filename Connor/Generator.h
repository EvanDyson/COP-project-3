#pragma once
#include <random>
#include <iostream>
#include <string>
#include "Names.h"
#include "City.h"

using namespace std;

struct Generator {

    //void generateCities(City (&c)[], int size);
    void generateCities(vector<City> &c, int count);

    int Random(int min, int max);
    int gaussianRandom(int mean, int stddev);
    double clip(double n, double lower, double upper);
    string generateCityName();
    //string generateState();
    int generatePopulation();
    int generateSize(int population);
    double generateDensity(int population, int size);
    int generateAvgHousingCost(int density);
    int generateAvgIncome(int housingCost);
    //int generateACL();

};




void Generator::generateCities(vector<City> &c, int count) {
    for (int i = 0; i < count; i++) {
        //string state = generateState();
        int pop = generatePopulation();
        int size = generateSize(pop);
        double density = generateDensity(pop, size);
        int house = generateAvgHousingCost(density);
        int income = generateAvgIncome(house);
        City city(generateCityName(), pop, size, density, house, income);
        c.push_back(city);
    }
}

int Generator::Random(int min, int max)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

int Generator::gaussianRandom(int mean, int stddev) {
    //Returns a random number that falls within a normal distribution
    //This means that if a large amount of random numbers are generated,
    //  they will form a bell curve centered around some mean.

    std::random_device dev;
    std::mt19937 rng(dev());

    std::normal_distribution<double> d(mean, stddev);

    return d(rng);
}

double Generator::clip(double n, double lower, double upper) {
    return std::max(lower, std::min(n, upper));
}

string Generator::generateCityName() {

    // City name combos:
    //  Prefix Middle       10%  10
    //  Prefix Suffix       10%  20
    //  Middle Suffix       30%  30-50
    //  Middle Fixed-Suffix     30%  60-80
    //  Prefix Fixed-Suffix     10%  90
    //  Prefix Middle Fixed-Suffix      5%  5
    //  Middle Fixed-Suffix Suffix      5%  5
    //
    //  Middle used most frequently,
    //      highest # of words in bank
    //
    //  Add standalone single-word names and
    //  Middle Fixed-Suffix Suffix

    string cityName;

    int r;

    r = Random(1, 100);

    if (r <= 10) {
        //cout << "Case 1" << endl;
        r = Random(0, prefixes.size()-1);
        cityName += prefixes[r] + " ";
        r = Random(0, middleWords.size()-1);
        cityName += middleWords[r];
    }
    else if (r <= 20) {
        //cout << "Case 2" << endl;
        r = Random(0, prefixes.size()-1);
        cityName += prefixes[r] + " ";
        r = Random(0, suffixes.size()-1);
        cityName += suffixes[r];
    }
    else if (r <= 50) {
        //cout << "Case 3" << endl;
        r = Random(0, middleWords.size()-1);
        cityName += middleWords[r] + " ";
        r = Random(0, suffixes.size()-1);
        cityName += suffixes[r];
    }
    else if (r <= 80) {
        //cout << "Case 4" << endl;
        r = Random(0, middleWords.size()-1);
        cityName += middleWords[r];
        r = Random(0, fixedSuffixes.size()-1);
        cityName += fixedSuffixes[r];
    }
    else if (r <= 90) {
        //cout << "Case 5" << endl;
        r = Random(0, prefixes.size()-1);
        cityName += prefixes[r];
        r = Random(0, fixedSuffixes.size()-1);
        cityName += fixedSuffixes[r];
    }
    else if (r <= 95) {
        //cout << "Case 6" << endl;
        r = Random(0, prefixes.size()-1);
        cityName += prefixes[r] + " ";
        r = Random(0, middleWords.size()-1);
        cityName += middleWords[r];
        r = Random(0, fixedSuffixes.size()-1);
        cityName += fixedSuffixes[r];
    }
    else {
        //cout << "Case 7" << endl;
        r = Random(0, middleWords.size()-1);
        cityName += middleWords[r];
        r = Random(0, fixedSuffixes.size()-1);
        cityName += fixedSuffixes[r] + " ";
        r = Random(0, suffixes.size()-1);
        cityName += suffixes[r];
    }

    return cityName;

}

/*string Generator::generateState() {
    int r = Random(0, 49);

    return stateAbbreviations[r];
};*/

int Generator::generatePopulation() {
    //Generates a random value based on a normal distribution with a mean of 3 mil and a standard deviation of 1 mil
    // and subtracts 2 mil to skew it to favor lower populations, then clips it so the population can't be lower than 5000.
    //int output = (int) clip(gaussianRandom(100000, 500000) - 100000, 5000, 10000000);
    //if (output == 5000) output = Random(1000, 7000);

    //Nested randoms create a skewed distribution where lower values are more likely
    return Random(500, Random(501, Random(502, 5000000)));
}

int Generator::generateSize(int population) {
    //Size is in square miles

    //return gaussianRandom(population);

    //Generates a size that is somewhat correlated with population
    /*int output = (int) (((double) population / 775.0) * pow(((double) (Random(1, 100)) / 10.0), 0.6));
    output = clip(Random(output - 100, output + 100), 10, 1000);
    if (output == 10) output = Random(10, 25);
    else if (output == 1000) output = Random(900, 1050);
    return output;*/

    //Generates a random size, skewed toward smaller sizes, similar to population
    return Random(10, Random(11,  Random(12, 1500)));
}

double Generator::generateDensity(int population, int size) {
    //Divides population by size to get density
    //Size is in square miles
    return ((double) population) / ((double) size);
}

int Generator::generateAvgHousingCost(int density) {

    //Generates an average housing cost that is correlated with population density
    /*int output = density * 400;
    output = clip(Random(output - 30000, output + 75000), 30000, 1500000);
    if (output == 30000) output = Random(25000, 35000);
    else if (output == 1500000) output = Random(1200000, 1600000);
    return output;*/

    //Generates a random average income, skewed toward lower values
    return Random(10000, Random(10001, Random(10002, 2500000)));
}

int Generator::generateAvgIncome(int housingCost) {

    //Generates an average income that is correlated to housing cost
    /*int output = housingCost / 12;
    output = clip(Random(output - 10000, output + 10000), 15000, 2000000);
    if (output == 15000) output = Random(10000, 30000);
    else if (output == 2000000) output = Random(1700000, 2100000);
    return output;*/

    //Generates a random average income, skewed toward lower values
    return Random(10000, Random(10001, Random(10002, 1000000)));
}