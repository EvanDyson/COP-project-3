//#include <iostream>   included in sort.h
//#include <vector>     included in sort.h
//#include "City.h"     included in sort.h
#include <fstream>
//#include <string>
#include <sstream>
#include "Sort.h"

// WILL BE USING CITIES RANDOM SKEWED BASED ON POP AND OTHER PARAMS
// "citiesRandomSkewed.csv"
int main()
{
    // setting up fstream to read the file
    fstream cFile;
    cFile.open("citiesRandomSkewed.csv", ios::in);
    // initialize the city array
    //vector<City*> cVector;
    
    // full size array
    //City* cArr[100000];
    // test array

    const unsigned int numCities = 100000;
    City* mergeArr = new City[numCities];
    City* radixArr = new City[numCities];

    // initialize a string to grab lines from csv file
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
            getline(str, population, ',');
            getline(str, density, ',');
            getline(str, avgHousingCost, ',');
            getline(str, size, ',');
            getline(str, avgIncome, ',');
            // save them into a city, in the constructor it sets the weights
            // City* pushMe(name, stoi(population), stoi(size), stod(density), stoi(avgHousingCost), stoi(avgIncome));
            pushMe->name = name;
            pushMe->population = stoi(population);
            pushMe->size = stoi(size);
            pushMe->density = stod(density);
            pushMe->avgHousingCost = stoi(avgHousingCost);
            pushMe->avgIncome = stoi(avgIncome);
            pushMe->setWeight();

            // push city onto the city vector to be sorted later
            // cVector.push_back(pushMe);

            // copy pushme into the array at the indexed city
            mergeArr[index] = *pushMe;
            radixArr[index] = *pushMe;
            index++;
        }
        cFile.close();
    }
    else
        cout << "Error opening file.\n";

    //mergeSort(cVector, 0, 5);
    //mergeSort(&mergeArr, 0, numCities - 1);
 
    //radixSort(&radixArr, numCities);
    
    /*
    still need to test sorting the city vector based on scores
    matrix works but radix has a bug where last thing is not sorted properly
     
    still need to test setting the weighted scores
    after both are successful integrate into GUI to get user inputs for wanted outputs to use for specific weighted scores.
    test setting specific parameters and see if expected outputs are seen
    */
    int option;
    cout << "Pick 1 for Merge sort. Pick 2 for Radix sort.\n";
    cin >> option;
    if (option == 1)
    {
        mergeSort(&mergeArr, 0, numCities - 1);
        // test printing the first 5 in the vector
        cout << "\t\t==============  MERGE SORT  ===============\n\n";
        for (int i = 0; i < numCities; i++)
        {
            //cout << "Name: " << cVector[i]->name << endl;
            //cout << "Population: " << cVector[i]->population << endl;
            //cout << "Density: " << cVector[i]->density << endl;
            //cout << "Size: " << cVector[i]->size << endl;
            //cout << "Avg. Income: " << cVector[i]->avgIncome << endl;
            //cout << "Avg. Housing Cost: " << cVector[i]->avgHousingCost << endl;
            //// current weighted score is pop / 2
            //cout << "Score: " << cVector[i]->weightScore << endl << endl;

            cout << "CITY #" << i + 1 << "'s name: " << mergeArr[i].name << endl;
            cout << "Population: " << mergeArr[i].population << endl;
            cout << "Density: " << mergeArr[i].density << endl;
            cout << "Size: " << mergeArr[i].size << endl;
            cout << "Avg. Income: " << mergeArr[i].avgIncome << endl;
            cout << "Avg. Housing Cost: " << mergeArr[i].avgHousingCost << endl;
            // current weighted score is pop / 2
            cout << "Score: " << mergeArr[i].weightScore << endl << endl;
        }
    }
    if (option == 2)
    {
        radixSort(&radixArr, numCities);
        cout << "\t\t==============  RADIX SORT  ===============\n\n";
        for (int i = 0; i < numCities; i++)
        {
            //cout << "Name: " << cVector[i]->name << endl;
            //cout << "Population: " << cVector[i]->population << endl;
            //cout << "Density: " << cVector[i]->density << endl;
            //cout << "Size: " << cVector[i]->size << endl;
            //cout << "Avg. Income: " << cVector[i]->avgIncome << endl;
            //cout << "Avg. Housing Cost: " << cVector[i]->avgHousingCost << endl;
            //// current weighted score is pop / 2
            //cout << "Score: " << cVector[i]->weightScore << endl << endl;

            cout << "CITY #" << i + 1 << "'s name: " << radixArr[i].name << endl;
            cout << "Population: " << radixArr[i].population << endl;
            cout << "Density: " << radixArr[i].density << endl;
            cout << "Size: " << radixArr[i].size << endl;
            cout << "Avg. Income: " << radixArr[i].avgIncome << endl;
            cout << "Avg. Housing Cost: " << radixArr[i].avgHousingCost << endl;
            // current weighted score is pop / 2
            cout << "Score: " << radixArr[i].weightScore << endl << endl;
        }
    }
}
