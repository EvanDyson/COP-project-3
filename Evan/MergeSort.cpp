#include "MergeSort.h"

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
    counter = indexL = indexR = 0;
    //creates index for the entire array, right subarray, and left subarray
    while (indexL < sizeL && indexR < sizeR)
    {
        if (rightSub[indexR]->getWeightScore() <= leftSub[indexL]->getWeightScore())
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

void mergeSort(vector<City*> arr, int left, int right)
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

void merge(vector<City*> arr, int left, int right)
{
    vector<City*> rightSub;
    vector<City*> leftSub;
    //creates representations of the right and left half of the array

    for (int i = 0; i < right - (left + (right - left) / 2); i++)
    {
        rightSub.at(i) = arr.at(i + 1 + left + (right - left) / 2);
    } //assigns right half array elements to right half representation
    for (int i = 0; i < (right - left) / 2 + 1; i++)
    {
        leftSub.at(i) = arr.at(i + left);
    } //assigns left half array elements to left half representation

    int counter, indexL, indexR;
    counter = indexL = indexR = 0;
    //creates index for the entire array, right subarray, and left subarray
    while (indexL < leftSub.size() && indexR < rightSub.size())
    {
        if (rightSub.at(indexR)->getWeightScore() <= leftSub.at(indexL)->getWeightScore())
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