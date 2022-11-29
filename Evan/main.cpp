#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "City.h"     
#include "Sort.h"
using namespace std;


// WILL BE USING CITIES RANDOM SKEWED BASED ON POP AND OTHER PARAMS
// "citiesRandomSkewed.csv"
int main()
{
    // setting up fstream to read the file
    fstream cFile;
    cFile.open("citiesRandomSkewed.csv", ios::in);
    // initialize the city vector
    vector<City> cVector;
    // initialize a string to grab lines from csv file
    string inLine;
    if (cFile.is_open())
    {   
        // grab first header line
        getline(cFile, inLine);
        while (getline(cFile, inLine))
        {
            stringstream str(inLine);
            // read in the parameters for each city
            string name, population, size, density, avgHousingCost, avgIncome;
            getline(str, name, ',');
            getline(str, population, ',');
            getline(str, size, ',');
            getline(str, density, ',');
            getline(str, avgHousingCost, ',');
            getline(str, avgIncome, ',');
            // save them into a city, in the constructor it sets the weights
            City pushMe(name, stoi(population), stoi(size), stod(density), stoi(avgHousingCost), stoi(avgIncome));

            // push city onto the city vector to be sorted later
            cVector.push_back(pushMe);
        }
        cFile.close();
    }
    else
        cout << "Error opening file.\n";



    
    // still need to test setting the weighted scores
    // still need to test sorting the city vector based on scores
    // after both are successful integrate into GUI to get user inputs for wanted outputs to use for specific weighted scores.
    // test setting specific parameters and see if expected outputs are seen



// test printing the first 5 in the vector
    for (int i = 0; i < 5; i++)
    {
        cout << "Name: " << cVector[i].name << endl;
        cout << "Population: " << cVector[i].population << endl;
        cout << "Density: " << cVector[i].density << endl;
        cout << "Size: " << cVector[i].size << endl;
        cout << "Avg. Income: " << cVector[i].avgIncome << endl;
        cout << "Avg. Housing Cost: " << cVector[i].avgHousingCost << endl;
        cout << "Score: " << cVector[i].weightScore << endl << endl;
    }

}