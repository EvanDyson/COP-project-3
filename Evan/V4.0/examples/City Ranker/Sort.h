#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include "City.h"

void mergeSort(City* arr[], int left, int right);
void merge(City* arr[], int left, int right);
//array based mergeSort;  mergeSort information aided by class slides

void mergeSort(vector<City*>& arr, int left, int right);
void merge(vector<City*>& arr, int left, int right);
//vector based mergeSort; mergeSort information aided by class slides

void radixSort(City* arr[], int size);
//array based radixMap sort

void radixSort(vector<City*>& arr);

void mergeSort(City* arr[], int left, int right)
{
    if (left == right)
        return;
    //divides array until only one element is left

    mergeSort(arr, left, left + (right - left) / 2);
    mergeSort(arr, left + (right - left) / 2 + 1, right);
    //continues to divide array

    merge(arr, left, right);
    //merges newly sorted halves of array
} //end of mergeSort

void merge(City* arr[], int left, int right)
{
    int sizeR = right - (left + (right - left) / 2);
    int sizeL = (right - left) / 2 + 1;
    //calculates size of the two halves of the array
    City** rightSub = new City * [sizeR];
    City** leftSub = new City * [sizeL];
    //creates representations of the right and left half of the array

    for (int i = 0; i < sizeR; i++)
    {
        rightSub[i] = arr[i + 1 + left + (right - left) / 2];
    } //assigns right half array elements to right half representation
    for (int i = 0; i < sizeL; i++)
    {
        leftSub[i] = arr[i + left];
    } //assigns left half array elements to left half representation

    int counter, indexL, indexR;
    counter = left;
    indexL = indexR = 0;
    //creates index for the entire array, right subarray, and left subarray
    while (indexL < sizeL && indexR < sizeR)
    {
        if (rightSub[indexR]->weightScore <= leftSub[indexL]->weightScore)
        {
            arr[counter] = rightSub[indexR];
            indexR++;
        }
        else
        {
            arr[counter] = leftSub[indexL];
            indexL++;
        }
        counter++;
    } //adds sorted elements to actual array

    while (indexR < sizeR)
    {
        arr[counter] = rightSub[indexR];
        indexR++;
        counter++;
    } //adds leftover right subarray elements if they exist
    while (indexL < sizeL)
    {
        arr[counter] = leftSub[indexL];
        indexL++;
        counter++;
    } //adds leftover left subarray elements if they exist

    delete[] leftSub;
    delete[] rightSub;
    //deletes the two subarray representations
} //end of merge


void mergeSort(vector<City*>& arr, int left, int right)
{
    if (left == right)
        return;
    //divides array until only one element is left

    mergeSort(arr, left, left + (right - left) / 2);
    mergeSort(arr, left + (right - left) / 2 + 1, right);
    //continues to divide array

    merge(arr, left, right);
    //merges newly sorted halves of array
} //end of mergeSort

void merge(vector<City*>& arr, int left, int right)
{
    vector<City*> rightSub;
    vector<City*> leftSub;
    //creates representations of the right and left half of the array
    int sizeR = right - (left + (right - left) / 2);
    int sizeL = (right - left) / 2 + 1;

    for (int i = 0; i < sizeR; i++)
    {
        rightSub.push_back(arr.at(i + 1 + left + (right - left) / 2));
    } //assigns right half array elements to right half representation
    for (int i = 0; i < sizeL; i++)
    {
        leftSub.push_back(arr.at(i + left));
    } //assigns left half array elements to left half representation

    int counter, indexL, indexR;
    counter = left;
    indexL = indexR = 0;
    //creates index for the entire array, right subarray, and left subarray
    while (indexL < leftSub.size() && indexR < rightSub.size())
    {
        if (rightSub.at(indexR)->weightScore <= leftSub.at(indexL)->weightScore)
        {
            arr.at(counter) = rightSub.at(indexR);
            indexR++;
        }
        else
        {
            arr.at(counter) = leftSub.at(indexL);
            indexL++;
        }
        counter++;
    } //adds sorted elements to actual array

    while (indexR < rightSub.size())
    {
        arr.at(counter) = rightSub.at(indexR);
        indexR++;
        counter++;
    } //adds leftover right subarray elements if they exist
    while (indexL < leftSub.size())
    {
        arr.at(counter) = leftSub.at(indexL);
        indexL++;
        counter++;
    } //adds leftover left subarray elements if they exist
} //end of merge

void radixSort(City* arr[], int size)
{
    int max = arr[0]->weightScore;
    for (int i = 1; i < size; i++)
    {
        if (arr[i]->weightScore > max)
            max = arr[i]->weightScore;
    }
    //finds whatever weight has the most digits

    for (int i = 1; max / i > 0; i *= 10)
    {
        vector<City*> output(size, nullptr);
        vector<int> count(10, 0);
        //creates vectors to collect sorted values and frequencies of digit

        for (int j = 0; j < size; j++)
        {
            count.at((arr[j]->weightScore / i) % 10)++;
        } //tracks how often specific digit appears
        count.at(0)--;

        for (int j = 1; j < count.size(); j++)
        {
            count.at(j) = count.at(j - 1) + count.at(j);
        } //adds counts

        for (int j = size - 1; j >= 0; j--)
        {
            output.at(count.at((arr[j]->weightScore / i) % 10)) = arr[j];
            count.at((arr[j]->weightScore / i) % 10)--;
        } //sorts values based on frequencies of digits

        for (int j = 0; j < size; j++)
        {
            arr[j] = output.at(j);
        } //puts output values back into the array
    } //end of for-loop that goes through every digit
} //end of radixSort

void radixSort(std::vector<City*> &arr)
{
    int max = arr.at(0)->getWeightScore();
    for (int i = 1; i < arr.size(); i++)
    {
        if (arr.at(i)->getWeightScore() > max)
            max = arr.at(i)->getWeightScore();
    }
    //finds whatever weight has the most digits

    for (int i = 1; max / i > 0; i *= 10)
    {
        std::vector<City*> output(arr.size(), nullptr);
        std::vector<int> count(10, 0);
        //creates vectors to collect sorted values and frequencies of digit

        for (int j = 0; j < arr.size(); j++)
        {
            count.at((arr.at(j)->getWeightScore() / i) % 10)++;
        } //tracks how often specific digit appears
        count.at(0)--;

        for (int j = 1; j < count.size(); j++)
        {
            count.at(j) = count.at(j - 1) + count.at(j);
        } //adds counts

        for (int j = arr.size() - 1; j >= 0; j--)
        {
            output.at(count.at((arr.at(j)->getWeightScore() / i) % 10)) = arr.at(j);
            count.at((arr.at(j)->getWeightScore() / i) % 10)--;
        } //sorts values based on frequencies of digits

        for (int j = 0; j < arr.size(); j++)
        {
            arr.at(j) = output.at(j);
        } //puts output values back into the array
    } //end of for-loop that goes through every digit
}
