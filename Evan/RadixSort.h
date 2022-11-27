#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "City.h"
using namespace std;

void radixSort(City* arr[], int size);
//array based radixMap sort

void radixSort(vector<City*> arr);
//vector based radixMap sort