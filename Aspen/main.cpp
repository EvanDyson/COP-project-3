#include "City.h"

void mergeSort (City** arr[], int left, int right);
void merge(City** arr[], int left, int right);
//array based mergeSort

//small edit to show GitHub example

void radixSort (City* arr[], int size);
//array based radixMap sort


void mergeSort (City** arr[], int left, int right)
{
    if (left == right)
        return;
    //divides array until only one element is left

    mergeSort(arr, left, left + (right-left)/2);
    mergeSort(arr, left + (right-left)/2 + 1, right);
    //continues to divide array

    merge(arr, left, right);
    //merges newly sorted halves of array
} //end of mergeSort

void merge (City** arr[], int left, int right)
{
    int sizeR = right - (left + (right-left)/2);
    int sizeL = (right-left)/2 + 1;
    //calculates size of the two halves of the array
    City** rightSub = new City*[sizeR];
    City** leftSub = new City*[sizeL];
    //creates representations of the right and left half of the array

    for (int i = 0; i < sizeR; i++)
    {
        rightSub[i] = *arr[i + 1 + left + (right - left)/ 2];
    } //assigns right half array elements to right half representation
    for (int i = 0; i < sizeL; i++)
    {
        leftSub[i] = *arr[i + left];
    } //assigns left half array elements to left half representation

    int counter, indexL, indexR;
    counter = left;
    indexL = indexR = 0;
    //creates index for the entire array, right subarray, and left subarray
    while (indexL < sizeL && indexR < sizeR)
    {
        if (rightSub[indexR]->getWeightScore() <= leftSub[indexL]->getWeightScore())
        {
            *arr[counter] = rightSub[indexR];
            indexR++;
        }
        else
        {
            *arr[counter] = leftSub[indexL];
            indexL++;
        }
        counter++;
    } //adds sorted elements to actual array

    while (indexR < sizeR)
    {
        *arr[counter] = rightSub[indexR];
        indexR++;
        counter++;
    } //adds leftover right subarray elements if they exist
    while (indexL < sizeL)
    {
        *arr[counter] = leftSub[indexL];
        indexL++;
        counter++;
    } //adds leftover left subarray elements if they exist

    delete[] leftSub;
    delete[] rightSub;
    //deletes the two subarray representations
} //end of merge

void radixSort (City* arr[], int size)
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
        std::vector<City*> output(size, nullptr);
        std::vector<int> count(10, 0);
        //creates vectors to collect sorted values and frequencies of digit

        for (int j = 0; j < size; j++)
        {
            count.at((arr[j]->getWeightScore()/i) % 10)++;
        } //tracks how often specific digit appears
        count.at(0)--;

        for  (int j = 1; j < count.size(); j++)
        {
            count.at(j) = count.at(j-1) + count.at(j);
        } //adds counts

        for (int j = size - 1; j >= 0; j--)
        {
            output.at(count.at((arr[j]->getWeightScore()/i) % 10)) = arr[j];
            count.at((arr[j]->getWeightScore()/i) % 10)--;
        } //sorts values based on frequencies of digits

        for (int j = 0; j < size; j++)
        {
            arr[j] = output.at(j);
        } //puts output values back into the array
    } //end of for-loop that goes through every digit
} //end of radixSort


int main() {
    City* l[30];
    l[0] = new City("3",1,1,1,1,1);
    l[0]->setWeightScore(334228);
    l[1] = new City("6",1,1,1,1,1);
    l[1]->setWeightScore(115510);
    l[2] = new City("2",1,1,1,1,1);
    l[2]->setWeightScore(40971);
    l[3] = new City("4",1,1,1,1,1);
    l[3]->setWeightScore(595434);
    l[4] = new City("1",1,1,1,1,1);
    l[4]->setWeightScore(919481);
    l[5] = new City("10",1,1,1,1,1);
    l[5]->setWeightScore(697731);
    l[6] = new City("9",1,1,1,1,1);
    l[6]->setWeightScore(919174);
    l[7] = new City("5",1,1,1,1,1);
    l[7]->setWeightScore(95303);
    l[8] = new City("8",1,1,1,1,1);
    l[8]->setWeightScore(440029);
    l[9] = new City("7",1,1,1,1,1);
    l[9]->setWeightScore(323083);
    l[10] = new City("7",1,1,1,1,1);
    l[10]->setWeightScore(90843);
    l[11] = new City("3",1,1,1,1,1);
    l[11]->setWeightScore(65508);
    l[12] = new City("6",1,1,1,1,1);
    l[12]->setWeightScore(139426);
    l[13] = new City("2",1,1,1,1,1);
    l[13]->setWeightScore(984692);
    l[14] = new City("4",1,1,1,1,1);
    l[14]->setWeightScore(129892);
    l[15] = new City("1",1,1,1,1,1);
    l[15]->setWeightScore(206977);
    l[16] = new City("10",1,1,1,1,1);
    l[16]->setWeightScore(147370);
    l[17] = new City("9",1,1,1,1,1);
    l[17]->setWeightScore(729310);
    l[18] = new City("5",1,1,1,1,1);
    l[18]->setWeightScore(127822);
    l[19] = new City("8",1,1,1,1,1);
    l[19]->setWeightScore(385750);
    l[20] = new City("7",1,1,1,1,1);
    l[20]->setWeightScore(419224);
    l[21] = new City("3",1,1,1,1,1);
    l[21]->setWeightScore(125235);
    l[22] = new City("6",1,1,1,1,1);
    l[22]->setWeightScore(585729);
    l[23] = new City("2",1,1,1,1,1);
    l[23]->setWeightScore(915163);
    l[24] = new City("4",1,1,1,1,1);
    l[24]->setWeightScore(471977);
    l[25] = new City("1",1,1,1,1,1);
    l[25]->setWeightScore(498089);
    l[26] = new City("10",1,1,1,1,1);
    l[26]->setWeightScore(884114);
    l[27] = new City("9",1,1,1,1,1);
    l[27]->setWeightScore(742282);
    l[28] = new City("5",1,1,1,1,1);
    l[28]->setWeightScore(756831);
    l[29] = new City("8",1,1,1,1,1);
    l[29]->setWeightScore(162755);
    for(City* elem:l)
        std::cout<<elem->getName();
    std::cout<<std::endl;
    radixSort(l, 30);
    for(City* elem:l)
        std::cout<<elem->getWeightScore() <<std::endl;

}
