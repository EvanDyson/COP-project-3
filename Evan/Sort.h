#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "City.h"
using namespace std;

struct Sort
{	
	//array based mergeSort
	void mergeSort(City* arr[], int left, int right);
	void merge(City* arr[], int left, int right);

	//vector based mergeSort; mergeSort information aided by class slides
	void mergeSort(vector<City*> arr, int left, int right);
	void merge(vector<City*> arr, int left, int right);


	//array based radixMap sort
	void radixSort(City* arr[], int size);

	//vector based radixMap sort
	void radixSort(vector<City*> arr);
};

