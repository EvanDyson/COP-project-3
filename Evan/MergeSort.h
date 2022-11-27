#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "City.h"
using namespace std;

void mergeSort(City* arr[], int left, int right);
void merge(City* arr[], int left, int right);
//array based mergeSort


void mergeSort(vector<City*> arr, int left, int right);
void merge(vector<City*> arr, int left, int right);
//vector based mergeSort; mergeSort information aided by class slides
