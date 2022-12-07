#pragma once
#include <fstream>
#include <sstream>
#include "Sort.h"
using namespace std;

// this is the function that combines the file of cities and their pregenerated data, and the sorting.
// it then returns a sorted vector for either merge sort or radix sort depending on what is selected.
vector<City*> CityRankers(int x, int pop_, int size_, float density_, int avgincome_, int avghousing_, bool inputWindowOpen, bool scannedYet)
{
    // 1 if merge sort the function also takes in if the input window is open and if this function has been ran yet for that specific result window that is displayed.
    if (x == 1 && !scannedYet && !inputWindowOpen)
    {
        fstream cFile;
        cFile.open("citiesRandomSkewed.csv", ios::in);
        const unsigned int numCities = 100000;
        vector<City*> mergeVector;
        string inLine;
        unsigned int index = 0;
        if (cFile.is_open())
        {
            // grab first header line
            getline(cFile, inLine);
            while (getline(cFile, inLine))
            {
                if (index == numCities)
                    break;
                City* pushMe = new City();
                stringstream str(inLine);
                // read in the parameters for each city
                string name, population, size, density, avgHousingCost, avgIncome;
                
                getline(str, name, ',');
                char* tempName = new char[name.length()];
                for (int g = 0; g < name.length(); g++)
                {
                    tempName[g] = name.at(g);
                    tempName[g + 1] = '\0';
                }
                getline(str, population, ',');
                getline(str, density, ',');
                getline(str, avgHousingCost, ',');
                getline(str, size, ',');
                getline(str, avgIncome, ',');
                pushMe->name = tempName;
                pushMe->population = stoi(population);
                pushMe->size = stoi(size);
                pushMe->density = stof(density);
                pushMe->avgHousingCost = stoi(avgHousingCost);
                pushMe->avgIncome = stoi(avgIncome);
                pushMe->setWeight(pop_, size_, density_, avgincome_, avghousing_);

                // push city onto the city vector to be sorted later
                mergeVector.push_back(pushMe);
                index++;
            }
            cFile.close();
        }
        else
            cout << "Error opening file.\n";

        mergeSort(mergeVector, 0, numCities - 1);
        return mergeVector;
    }
    // 2 if radix sort the function also takes in if the input window is open and if this function has been ran yet for that specific result window that is displayed.
    else if (x == 2 && !scannedYet && !inputWindowOpen)
    {
        fstream cFile;
        cFile.open("citiesRandomSkewed.csv", ios::in);
        const unsigned int numCities = 100000;
        vector<City*> radixVector;
        string inLine;
        unsigned int index = 0;
        if (cFile.is_open())
        {
            // grab first header line
            getline(cFile, inLine);
            while (getline(cFile, inLine))
            {
                if (index == numCities)
                    break;
                City* pushMe = new City();
                stringstream str(inLine);
                // read in the parameters for each city
                string name, population, size, density, avgHousingCost, avgIncome;

                getline(str, name, ',');
                char* tempName = new char[name.length()];
                for (int g = 0; g < name.length(); g++)
                {
                    tempName[g] = name.at(g);
                    tempName[g + 1] = '\0';
                }
                getline(str, population, ',');
                getline(str, density, ',');
                getline(str, avgHousingCost, ',');
                getline(str, size, ',');
                getline(str, avgIncome, ',');
                pushMe->name = tempName;
                pushMe->population = stoi(population);
                pushMe->size = stoi(size);
                pushMe->density = stof(density);
                pushMe->avgHousingCost = stoi(avgHousingCost);
                pushMe->avgIncome = stoi(avgIncome);
                pushMe->setWeight(pop_, size_, density_, avgincome_, avghousing_);
                radixVector.push_back(pushMe);
                index++;
            }
            cFile.close();
        }
        else
            cout << "Error opening file.\n";

        radixSort(radixVector);
        return radixVector;
    }
    else
    { }
}
