# Welcome to our City Ranker project files folder!
#### Please note that these files will not run without first downloading Dear ImGui.
##### This is just for visualization purposes to see the code that our program is using.

citiesRandomSkewed.csv is the file that contains our 100,000 data points. the csv file contains 100,000 unique cities that were developed using an algorithm that would generate a population, and then skew the other numbers towards the population. This was created by Connor. 

main.cpp is the file that contains all the code that controls the GUI. This code is not original, it was modified to our uses. Everything used in this file is open-source and licensed under MIT license. 

Sort.h contains all the code used in our merge and radix sorting algorithms. This was created by Aspen. 

City.h Contains our City struct and all the functions used by the city. This was created by Connor and modified by Evan. 

CityRanker.h contains a single function that is used by the GUI to call and get a sorted vector in return. The function takes in an int that is used to detect whether radix or merge sort is selected. The csv file is then read and the vector is created. That vector is sent to a sorting algorithm and then is returned. This was created by Evan. 

Tower.jpg is the background image. This was provided by UF.