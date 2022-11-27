#include "RadixSort.h"

void radixSort(City* arr[], int size)
{
    int max = arr[0]->getWeightScore();
    for (int i = 1; i < size; i++)
    {
        if (arr[i]->getWeightScore() > max)
            max = arr[i]->getWeightScore();
    }
    //finds whatever weight has the most digits

    for (int i = 1; max / i > 0; i *= 10)
    {
        vector<City*> output(size, nullptr);
        vector<int> count(10, 0);
        //creates vectors to collect sorted values and frequencies of digit

        for (int j = 0; j < size; j++)
        {
            count.at((arr[j]->getWeightScore() / i) % 10)++;
        } //tracks how often specific digit appears
        count.at(0)--;

        for (int j = 1; j < count.size(); j++)
        {
            count.at(j) = count.at(j - 1) + count.at(j);
        } //adds counts

        for (int j = size - 1; j >= 0; j--)
        {
            output.at(count.at((arr[j]->getWeightScore() / i) % 10)) = arr[j];
            count.at((arr[j]->getWeightScore() / i) % 10)--;
        } //sorts values based on frequencies of digits

        for (int j = 0; j < size; j++)
        {
            arr[j] = output.at(j);
        } //puts output values back into the array
    } //end of for-loop that goes through every digit
} //end of radixSort

void radixSort(vector<City*> arr) {
    int max = arr.at(0)->getWeightScore();
    for (int i = 1; i < arr.size(); i++) {
        if (arr.at(i)->getWeightScore() > max)
            max = arr.at(i)->getWeightScore();
    } //finds whatever weight has the most digits

    for (int i = 1; max / i > 0; i *= 10) //change i according to how many digits we want
    {
        vector<City*> output(arr.size(), nullptr);
        vector<int> count(10, 0);
        //creates vectors to collect sorted values and frequencies of digit

        for (int j = 0; j < arr.size(); j++) {
            count.at((arr.at(j)->getWeightScore() / i) % 10)++;
        } //tracks how often specific digit appears
        count.at(0)--;

        for (int j = 1; j < count.size(); j++) {
            count.at(j) = count.at(j - 1) + count.at(j);
        } //adds counts

        for (int j = arr.size() - 1; j >= 0; j--) {
            output.at(count.at((arr.at(j)->getWeightScore() / i) % 10)) = arr.at(j);
            count.at((arr.at(j)->getWeightScore() / i) % 10)--;
        } //sorts values based on frequencies of digits

        for (int j = 0; j < arr.size(); j++) {
            arr.at(j) = output.at(j);
        } //puts output values back into the array
    } //end of for-loop that goes through every digit
} //end of radixSort
